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

    if ( argc != 2 ) {
        std::cout << "Usage: midi_report device_name\n";
        exit(1);
    }    

    Container synth;
    MidiInput midi(argv[1]);
    Audio audio;

    synth.add(midi).add(audio);

    synth.listen(MIDI_ANY, [] (const Event &e) {
        std::cout << e.code << ", " << e.id << ", " << e.value << "\n";
    });

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}

