#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Synthesizer synth;     
    OscGroup osc;
    Audio audio;
    Midi midi;
    Envelope env;
    Biquad filter;
    Gain gain;
    
    synth.add(osc)     .add(audio)     .add(midi)
         .add(env)     .add(filter)    .add(gain);

    // Main connections
    synth.connect( osc,    "signal", filter, "signal")
         .connect( filter, "signal", env,    "signal")
         .connect( env,    "signal", gain,   "signal")
         .connect( gain,   "signal", audio,  "left")
         .connect( gain,   "signal", audio,  "right");

    // Midi Keyboard control
    int num_keys = 0;
    int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
    synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               osc.set_input("frequency", e.frequency());
               filter.set_input("offset", e.frequency());
               filter.recalculate(); // TODO: This is awkward and could be forgotten
               env.set_input("velocity", e.value / 127.0);
               env.trigger();
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

     // Oscillator selector control
     synth.listen(MIDI_MOD, [&] (const Event &e) {
         if ( e.id == 16 ) {
              osc.select(e.value / 127.0);
         }
     });

     // Faders     Midi mappings
     //            module  input        min   max    inver?   midi id
     synth.control(filter, "frequency", 1000, 6000,  false,   56)
          .control(filter, "resonance", 0.1,  20,    false,   60)
          .control(gain,   "amplitude", 0,    1,     false,   62)
          .control(osc,    "amplitude", 0,    1,     false,   19)
          .control(env,    "attack",    0.1,  25,    true,    49)
          .control(env,    "decay",     0.1,  25,    true,    53)
          .control(env,    "sustain",   0,    1,     false,   57)
          .control(env,    "release",   0.1,  25,    true,    61);

     // MIDI Buttons
     int akai_port = midi.get_port_id("MIDI Mix");
     synth.button(akai_port, 13, [&] (const Event &e) { filter.set_type("lpf");   })
          .button(akai_port, 16, [&] (const Event &e) { filter.set_type("hpf");   })
          .button(akai_port, 19, [&] (const Event &e) { filter.set_type("apf");   })
          .button(akai_port, 22, [&] (const Event &e) { filter.toggle();          });

     synth.run(FOREVER);

     return 0;

}