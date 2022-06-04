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

    MidiInput midi("MIDI Mix");
    Buttons buttons("MIDI Mix");
    Audio audio; // need to throttle execution
    Container synth;
    
    synth.add(midi)
         .add(audio)
         .add(buttons)
         .propagate_to(buttons);

    buttons.momentary(1, [&] (const Event &e) {
        std::cout << "1\n";
        buttons.blink_on(12, 0.5);
    })
    .momentary(3, [&] (const Event &e) {
        std::cout << "3\n";
        buttons.blink_off(12);
    });    

    buttons.mutex({4,7,6,9}, {
      [] (const Event &e) { std::cout << "4\n"; },
      [] (const Event &e) { std::cout << "7\n"; },
      [] (const Event &e) { std::cout << "6\n"; },
      [] (const Event &e) { std::cout << "9\n"; }
    });

    buttons.blink_on(10, 0.25);

    synth.run(UNTIL_INTERRUPTED);

    buttons.clear_leds(); 

    return 0;

}