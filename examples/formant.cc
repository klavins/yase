#include <iostream>
#include <fstream>
#include "json.hpp"
#include "yase.hh"

using namespace yase;
using namespace nlohmann;

int main(int argc, char * argv[]) {

    // Components
    Synthesizer synth("MIDI Mix"); 

    Buzz osc;
    Envelope env;
    Midi midi;
    Audio audio;
    Resonator formant[3];
    Biquad lpf, hpf;
    Mixer mixer(3);
    Gain gain;
    Compressor compressor;

    std::ifstream config_stream("config/akai-monophonic.json");
    json config, midi_map;
    config_stream >> config; 
    midi_map = config["midi_ids"];    

    synth.add(osc)
         .add(env)
         .add(lpf)
         .add(hpf)
         .add(mixer)
         .add(midi)
         .add(audio)
         .add(gain)
         .add(compressor);

    lpf.set_type("lpf");
    lpf.set_type("hpf");

    int freq_id[] = { 16, 17, 18 },
        res_id[] = { 20, 21, 22 },
        amp_id[] = { 24, 25, 26 };

    for ( int i=0; i<3; i++ ) {
      synth.add(formant[i]);
      synth.connect(osc, "signal", formant[i], "signal");
      synth.connect(formant[i], "signal", mixer, i );
      synth.control(formant[i], "radius", 0, 0.99, freq_id[i]);
      synth.control(formant[i], "theta", 0, 2*3.14159, res_id[i]);      
      synth.control(mixer, mixer.amplitude_index(i), 0, 0.33, amp_id[i]);
    }

    synth.connect(mixer, "signal", env, "signal");
    synth.connect(env, "signal", lpf, "signal" );
    synth.connect(lpf, "signal", hpf, "signal" );
    synth.connect(hpf, "signal", gain, "signal" );

    synth.connect(gain, "signal", audio, "left");
    synth.connect(gain, "signal", audio, "right");

    // Keyboard Control
    int num_keys = 0;
    int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
    synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               osc.set_input("frequency", e.frequency());
               env.set_input("velocity", e.value / 127.0);
               env.trigger();
            //    for ( int i=0; i<3; i++ ) {
            //        formant[i].set_input("offset", e.frequency());
            //    }               
               lpf.set_input("offset", e.frequency());
               num_keys++;
          }
     })
     .listen(MIDI_KEYUP, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               num_keys--;
               if ( num_keys == 0 ) {
                    env.release();
               }
          }
     });   

    synth.control(env, "attack",  0.005, 1, midi_map["env"]["A"])             // ENVELOPE
         .control(env, "decay",   0.005, 1, midi_map["env"]["D"])
         .control(env, "sustain", 0,     1, midi_map["env"]["S"])
         .control(env, "release", 0.005, 1, midi_map["env"]["R"]);

    synth.control(hpf, "frequency", 50, 20000, 57);            // LPF
    synth.control(hpf, "resonance", 0.1, 25, 61);

    synth.control(lpf, "frequency", 50, 20000, 49);            // HPF
    synth.control(lpf, "resonance", 0.1, 25, 53);

    synth.control(gain, "amplitude", 0, 0.125, midi_map["volume"]);            // VOLUME         

    synth.run(UNTIL_INTERRUPTED);

    exit(0);

}