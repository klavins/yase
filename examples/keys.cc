#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Synthesizer synth;     

    Saw osc;
    Audio audio;
    Midi midi;
    Envelope env;
    Biquad filter;
    Gain gain;

    filter.set_type("lpf");

    Fader cutoff(1000,6000), res(0.1,20), volume(0,1),
          attack(0.001, 5),
          decay(0.001, 5),
          sustain(0,1),
          release(0.001,5);
    
    osc.set_type("additive");

    synth.add(osc)     .add(audio)     .add(midi)
         .add(env)     .add(filter)    .add(cutoff)
         .add(res)     .add(volume)    .add(gain)
         .add(attack)  .add(decay)     .add(sustain)
         .add(release);

    // Main connections
    synth.connect( osc,    "signal", filter, "signal")
         .connect( filter, "signal", env,    "signal")
         .connect( env,    "signal", gain,   "signal")
         .connect( gain,   "signal", audio,  "left")
         .connect( gain,   "signal", audio,  "right");

     // Fader connections
     synth.connect( cutoff, "value",  filter, "frequency")
          .connect( res,    "value",  filter, "resonance")
          .connect( volume, "value",  gain,   "amplitude")
          .connect( attack, "value",  env,    "attack")
          .connect( decay,  "value",  env,    "decay")
          .connect( sustain, "value", env,    "sustain")
          .connect( release, "value", env,    "release");

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

     // Fader - Midi mappings
     synth.control(cutoff, 56)
          .control(res, 60)
          .control(volume, 62)
          .control(attack, 19)
          .control(decay, 23)
          .control(sustain, 27)
          .control(release, 31);

     // MIDI Buttons
     int akai_port = midi.get_port_id("MIDI Mix");
     synth.button(akai_port, 1,  [&] (const Event &e) { osc.set_type("raw");      })
          .button(akai_port, 4,  [&] (const Event &e) { osc.set_type("ptr1");     })
          .button(akai_port, 7,  [&] (const Event &e) { osc.set_type("additive"); })
          .button(akai_port, 13, [&] (const Event &e) { filter.set_type("lpf");   })
          .button(akai_port, 16, [&] (const Event &e) { filter.set_type("hpf");   })
          .button(akai_port, 19, [&] (const Event &e) { filter.set_type("apf");   })
          .button(akai_port, 22, [&] (const Event &e) { filter.toggle();          });

     synth.run(FOREVER);

     return 0;

}