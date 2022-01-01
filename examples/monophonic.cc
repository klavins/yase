#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "yase.hh"

using namespace yase;
using namespace nlohmann;

int main(int argc, char * argv[]) {

    // Components
    Synthesizer synth; 
    OscGroup osc[3];
    Sine lfo;
    Mixer mixer(3),
          mod_mixer1(2),
          mod_mixer2(2),
          filter_env_mixer(2);
    Audio audio;
    Midi midi;
    Envelope env, filter_env;
    Biquad filter;
    Gain gain,
         osc2_lfo_gain;
    Sequencer seq;
    
    synth.add(osc[0])    .add(osc[1])      .add(osc[2])
         .add(mixer)     .add(mod_mixer1)  .add(mod_mixer2)
         .add(audio)     .add(midi)        .add(lfo)
         .add(env)       .add(filter)      .add(gain)
         .add(osc2_lfo_gain) 
         .add(filter_env_mixer)
         .add(filter_env)
         .add(seq);

    std::ifstream config_stream("config/akai-monophonic.json");
    json config, midi_map;
    config_stream >> config; 
    midi_map = config["midi_ids"];

    // Connections 
    for ( int i=0; i<3; i++ ) {
         synth.connect(osc[i], "signal", mixer, i);
    }
    synth.connect( mixer,  "signal", filter, "signal")
         .connect( filter, "signal", env,    "signal")
         .connect( env,    "signal", gain,   "signal")
         .connect( gain,   "signal", audio,  "left")
         .connect( gain,   "signal", audio,  "right");

    synth.connect( lfo,    "signal", mod_mixer1, 0 )
         .connect( osc[0], "signal", mod_mixer1, 1 )
         .connect( mod_mixer1, "signal", osc[1], "modulation" )

         .connect( lfo,    "signal", mod_mixer2, 0 )
         .connect( osc[1], "signal", mod_mixer2, 1 )
         .connect( mod_mixer2, "signal", osc[2], "modulation" );

     synth.connect( lfo, "signal", osc2_lfo_gain, "signal" )
          .connect( osc2_lfo_gain, "signal", osc[0], "modulation" );

     synth.connect( filter_env, "signal", filter_env_mixer, 1 )
          .connect( filter_env_mixer, "signal", filter, "frequency" );
     
     filter_env_mixer.set_input(0, 1);

    // Keyboard Control
    std::vector<int> keys;
    int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
    synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               for ( int i=0; i<3; i++ ) {
                   osc[i].set_input("frequency", e.frequency());
               }
               filter.set_input("offset", e.frequency());
               filter.recalculate(); // TODO: filter could intercept set_input
               env.set_input("velocity", e.value / 127.0);
               env.trigger();
               filter_env.trigger();
               keys.push_back(e.id);
          }
     })
     .listen(MIDI_KEYUP, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               keys.erase(std::remove(keys.begin(), keys.end(), e.id), keys.end());
               if ( keys.size() == 0 ) {
                    env.release();
                    filter_env.release();
               } else {
                    Event temp(MIDI_KEYDOWN, keys.back(), 0, 0);
                    for ( int i=0; i<3; i++ ) {
                       osc[i].set_input("frequency", temp.frequency());
                    }
                    filter.set_input("offset", temp.frequency());
                    filter.recalculate(); // TODO: filter could intercept set_input                    
               }
          }
     });

     // Oscillator selector control
     for ( int i=0; i<3; i++ ) {
         synth.listen(MIDI_MOD, [&,i] (const Event &e) {
             if ( e.id == midi_map["osc_selectors"][i] ) osc[i].select(e.value / 127.0);
         });
     }

     // Faders
     synth.control(lfo, "frequency", 0.01, 10, midi_map["lfo_freq"])           // LFO
          .control(lfo, "amplitude", 0, 10,    midi_map["lfo_amp"])

          .control(mod_mixer1, mod_mixer1.amplitude_index(0), 0, 5, midi_map["mod_mixer_lfo_ctrl"][0])    // OSC CONNECTIONS 
          .control(mod_mixer1, mod_mixer1.amplitude_index(1), 0, 5, midi_map["mod_mixer_mod_ctrl"][0])
          .control(mod_mixer2, mod_mixer2.amplitude_index(0), 0, 5, midi_map["mod_mixer_lfo_ctrl"][1])
          .control(mod_mixer2, mod_mixer2.amplitude_index(1), 0, 5, midi_map["mod_mixer_mod_ctrl"][1])
          .control(osc2_lfo_gain, "amplitude", 0, 10, 50)

          .control(filter, "resonance", 0.1, 20, midi_map["filter_resonance"]) // FILTER

          .control(filter_env, "attack",  0.005, 1, midi_map["filter_env"]["A"])
          .control(filter_env, "decay",   0.005, 1, midi_map["filter_env"]["D"])
          .control(filter_env, "sustain", 0,     1, midi_map["filter_env"]["S"])
          .control(filter_env, "release", 0.005, 1, midi_map["filter_env"]["R"])

          .control(filter_env_mixer, filter_env_mixer.amplitude_index(0), 10, 6000, midi_map["filter_freq"] )
          .control(filter_env_mixer, filter_env_mixer.amplitude_index(1), 10, 6000, midi_map["filter_eg_amt"] )

          .control(env, "attack",  0.005, 1, midi_map["env"]["A"])             // ENVELOPE
          .control(env, "decay",   0.005, 1, midi_map["env"]["D"])
          .control(env, "sustain", 0,     1, midi_map["env"]["S"])
          .control(env, "release", 0.005, 1, midi_map["env"]["R"])

          .control(gain, "amplitude", 0, 0.25, midi_map["volume"]);            // VOLUME

     // Oscillator amplitudes and tuning
     for (int i=0; i<3; i++) {
         synth.control(mixer, i+3, 0, 1,          midi_map["amplitudes"][i])
              .control(osc[i], "tuning", -7, 8,   midi_map["tunings"][i])
              .control(osc[i], "harmonic", -2, 3, midi_map["harmonics"][i]);
     }

     // MIDI Buttons
     int akai_port = midi.get_port_id("MIDI Mix");
     
     midi.on(akai_port, midi_map["buttons"]["lpf"])
         .on(akai_port, midi_map["buttons"]["filter_toggle"])
         .off(akai_port, midi_map["buttons"]["hpf"]);

     synth.button(akai_port, midi_map["buttons"]["lpf"], [&] (const Event &e) { 
              filter.set_type("lpf");   
              midi.on(akai_port, midi_map["buttons"]["lpf"])
                  .off(akai_port, midi_map["buttons"]["hpf"]);
          })
          .button(akai_port, midi_map["buttons"]["hpf"], [&] (const Event &e) {
              filter.set_type("hpf");
              midi.off(akai_port, midi_map["buttons"]["lpf"])
                  .on(akai_port, midi_map["buttons"]["hpf"]);
          })
          .button(akai_port, midi_map["buttons"]["filter_toggle"], [&] (const Event &e) {
              if ( filter.toggle() ) {
                   midi.on(akai_port, midi_map["buttons"]["filter_toggle"]);
              } else {
                   midi.off(akai_port, midi_map["buttons"]["filter_toggle"]);
              }
          });

     // Crazy randomize button
     synth.button(akai_port, midi_map["buttons"]["randomize"], [&] (const Event &e) {
          synth.randomize_faders();
          gain.set_input("amplitude", 0.1);
     });

     // Sequencer setup
     synth.listen(MIDI_KEYDOWN, [&] (const Event &e) { 
             if ( e.port == keyboard_port ) {
                  seq.keydown(e); 
             }
          })
          .listen(MIDI_KEYUP, [&] (const Event &e) { 
             if ( e.port == keyboard_port ) {
                 seq.keyup(e); 
             }
          })
          .button(akai_port, midi_map["buttons"]["rest"], [&] (const Event &e) {
               seq.insert_rest();
          })
          .button(akai_port, midi_map["buttons"]["reset"], [&] (const Event &e) { 
               seq.reset(); 
          })
          .button(akai_port, midi_map["buttons"]["record"], [&] (const Event &e) { 
               seq.record();
               midi.on(akai_port, midi_map["buttons"]["record"])
                   .off(akai_port, midi_map["buttons"]["play"]);
          })
          .button(akai_port, midi_map["buttons"]["stop"], [&] (const Event &e) { 
               seq.stop(); 
               midi.off(akai_port, midi_map["buttons"]["record"])
                   .off(akai_port, midi_map["buttons"]["play"]);
          })
          .button(akai_port, midi_map["buttons"]["play"], [&] (const Event &e) { 
               seq.play(); 
               midi.off(akai_port, midi_map["buttons"]["record"])
                   .on(akai_port, midi_map["buttons"]["play"]);
          })
          .button(akai_port, midi_map["buttons"]["clear"], [&] (const Event &e) { 
               seq.clear(); 
          })
          .button(akai_port, midi_map["buttons"]["decrease_tempo"], [&] (const Event &e) {
               double tempo = seq.get_input("tempo");
               if ( tempo > 20 ) {
                    seq.set_input("tempo", tempo - 20);
               }
          })
          .button(akai_port, midi_map["buttons"]["increase_tempo"], [&] (const Event &e) {
               double tempo = seq.get_input("tempo");
               seq.set_input("tempo", tempo + 20);               
          })
          .button(akai_port, midi_map["buttons"]["decrease_duration"], [&] (const Event &e) {
               double dur = seq.get_input("duration");
               if ( dur > 0.2 ) {
                    seq.set_input("duration", dur - 0.1);
               }
          })
          .button(akai_port, midi_map["buttons"]["increase_duration"], [&] (const Event &e) {
               double dur = seq.get_input("duration");
               if ( dur < 0.9 ) {
                    seq.set_input("duration", dur + 0.1);
               }
          });          

     // Go!
     synth.run(UNTIL_INTERRUPTED);

     // Shutdown
     for ( auto& [_, id] : midi_map["buttons"].items() ) {
       midi.off(akai_port, id);
     }

     return 0;

}