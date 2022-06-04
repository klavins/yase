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

    Saw osc1;
    Saw osc2;
    Controls controls;
    Audio audio;
    MidiInput midi("Arturia MiniLab mkII");
    Container synth;

    osc1.set_input("frequency", 55);
    osc2.set_input("frequency", 55);

    synth.add(midi)
         .add(controls) 
         .propagate_to(controls);

    synth.connect(osc1,"signal",audio,"left")
         .connect(osc2,"signal",audio,"right");

    controls.map(osc1, "frequency", 55, 110, 74)
            .map(osc2, "frequency", 55, 110, 71);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}
