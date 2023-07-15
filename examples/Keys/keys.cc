// 
// YASE Example
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    json config = get_config("config/akai-monophonic.json");

    Container synth;
    Buttons buttons(config["controller"]);     
    Controls controls;
    AdditiveSaw osc;
    Sine lfo;
    Audio audio;
    MidiInput midi_keyboard(config["keyboard"]),
              midi_controller(config["controller"]);
    Envelope env;
    Biquad filter;
    Gain gain;

    filter.set_type("lpf");   

    synth.add(osc)     
         .add(audio)
         .add(buttons)
         .add(env)
         .add(filter)
         .add(lfo)
         .add(gain)
         .add(midi_keyboard)
         .add(midi_controller)         
         .add(controls)
         .propagate_to(buttons)
         .propagate_to(controls);

    synth.connect( osc,    "signal", filter, "signal")
         .connect( filter, "signal", env,    "signal")
         .connect( env,    "signal", gain,   "signal")
         .connect( gain,   "signal", audio,  "left")
         .connect( gain,   "signal", audio,  "right");

    std::vector<int> keys;
    synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
          if ( e.port == midi_keyboard.port() ) {
               osc.set_input("frequency", e.frequency());
               filter.set_input("offset", e.frequency());
               filter.recalculate(); // TODO: This is awkward and could be forgotten
               env.set_input("velocity", e.value / 127.0);
               env.trigger();
               keys.push_back(e.id);
          }
     })
     .listen(MIDI_KEYUP, [&] (const Event &e) {
          if ( e.port == midi_keyboard.port() ) {
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

     controls.map(env, "attack",  0.005, 1, 19)
             .map(env, "decay",   0.005, 1, 23)
             .map(env, "sustain", 0,     1, 27)
             .map(env, "release", 0.005, 1, 31)
             .map(gain, "amplitude", 0, 0.25, 62)
             .map(filter, "frequency", 1000, 6000, 56)
             .map(filter, "resonance", 0.1, 20, 60);

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