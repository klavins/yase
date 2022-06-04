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
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "yase.hh"
#include "mono.hh"

using namespace yase;
using namespace nlohmann;

int main(int argc, char * argv[]) {

    json config = get_config("./examples/Multi/akai.json");

    // Components
    Container synth;
    Buttons buttons(config["controller"]); 
    MidiInput keyboard(config["keyboard"]),
              controller(config["controller"]);
   
    Audio audio;  
    Mixer mixer(4);  
    Mono mono[]{
         Mono(keyboard, controller, config), 
         Mono(keyboard, controller, config), 
         Mono(keyboard, controller, config), 
         Mono(keyboard, controller, config)
    };

    synth.add(keyboard)
         .add(controller)
         .add(audio)
         .add(mixer)
         .add(buttons)
         .propagate_to(buttons);

    for ( int i=0; i<4; i++ ) {
         synth.add(mono[i]);
         synth.connect(mono[i], "signal", mixer, i);
    }

    synth.connect(mixer, "signal", audio, "left");
    synth.connect(mixer, "signal", audio, "right"); 

    // Select synth
    int current = 0;

    buttons.mutex({15,18,21,24}, {
        [&] (const Event &e) { current = 0; },
        [&] (const Event &e) { current = 1; },
        [&] (const Event &e) { current = 2; },
        [&] (const Event &e) { current = 3; }
    });

    // Interesting situation where just doing propagate_to
    // at the beginning won't work. We only want to send
    // events to the active container. 
    synth.listen(MIDI_ANY, [&] (const Event &e) {
         mono[current].inject(e); 
    });   

    // Go!
    synth.run(UNTIL_INTERRUPTED);

    // Shutdown
    buttons.clear_leds();

    for ( auto &s : mono ) {
         mono->clear_leds();
    }

    return 0;

}