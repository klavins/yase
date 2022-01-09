#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    json config = get_config("config/akai-monophonic.json");

    Container synth;
    ButtonManager buttons(config["controller"]);     
    FaderManager controls;
    Saw osc;
    Sine lfo;
    Audio audio;
    Midi midi;
    Envelope env;
    Biquad filter;
    Gain gain;

    filter.set_type("lpf");   
    osc.set_type("additive");

    synth.add(osc)     
         .add(audio)
         .add(midi)
         .add(buttons)
         .add(env)
         .add(filter)
         .add(lfo)
         .add(gain)
         .add(buttons)
         .add(controls)
         .propagate_to(buttons)
         .propagate_to(controls);

    synth.connect( osc,    "signal", filter, "signal")
         .connect( filter, "signal", env,    "signal")
         .connect( env,    "signal", gain,   "signal")
         .connect( gain,   "signal", audio,  "left")
         .connect( gain,   "signal", audio,  "right");

    std::vector<int> keys;
    int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
    synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               osc.set_input("frequency", e.frequency());
               filter.set_input("offset", e.frequency());
               filter.recalculate(); // TODO: This is awkward and could be forgotten
               env.set_input("velocity", e.value / 127.0);
               env.trigger();
               keys.push_back(e.id);
          }
     })
     .listen(MIDI_KEYUP, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               keys.erase(std::remove(keys.begin(), keys.end(), e.id), keys.end());
               if ( keys.size() == 0 ) {
                    env.release();
               } else {
                    Event temp(MIDI_KEYDOWN, keys.back(), 0, 0);
                    osc.set_input("frequency", temp.frequency());
                    filter.set_input("offset", temp.frequency());
                    filter.recalculate(); 
               }
          }
     });

     controls.control(env, "attack",  0.005, 1, 19)
             .control(env, "decay",   0.005, 1, 23)
             .control(env, "sustain", 0,     1, 27)
             .control(env, "release", 0.005, 1, 31)
             .control(gain, "amplitude", 0, 0.25, 62)
             .control(filter, "frequency", 1000, 6000, 56)
             .control(filter, "resonance", 0.1, 20, 60);

     buttons.mutex({1, 4, 7}, {
          [&] (const Event &e) { osc.set_type("raw");      },
          [&] (const Event &e) { osc.set_type("ptr1");     },
          [&] (const Event &e) { osc.set_type("additive"); }
     });

     buttons.mutex({3, 6, 9}, {
          [&] (const Event &e) { filter.set_type("lpf");   },
          [&] (const Event &e) { filter.set_type("hpf");   },
          [&] (const Event &e) { filter.set_type("apf");   }
     });

     buttons.toggle(22, [&] (const Event &e) { filter.toggle(); }, true);

     synth.run(UNTIL_INTERRUPTED);
     buttons.clear_leds();

     return 0;

}