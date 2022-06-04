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

    Clock clock;
    Audio audio(3);
    MidiInput midi("MIDI Mix");
    Controls controls;
    Container container;
    
    clock.set_input("frequency", 4);

    container.add(clock)
             .add(midi)
             .add(audio)
             .add(controls)
             .connect(clock,"signal",audio,"aux1")
             .propagate_to(controls);

    controls.map(clock, "frequency", 1, 10, 60);

    container.run(UNTIL_INTERRUPTED);

    return 0;

}