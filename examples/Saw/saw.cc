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

    Saw raw, aliased, additive;
    AntiAlias anti_aliased(aliased);

    raw.set_type("raw");
    aliased.set_type("aliased");
    additive.set_type("additive");
    additive.set_input("amplitude", 0.7);

    Audio audio;
    Container synth; 

    double freq = B6;
    raw.set_input("frequency", freq);
    aliased.set_input("frequency", freq);
    additive.set_input("frequency", freq);

    synth.connect(raw,"signal",audio,"left")
         .connect(raw,"signal",audio,"right");

    std::cout << "raw\n";
    synth.run(SAMPLE_RATE);    

    vector<Module *> oscs = { &raw, &anti_aliased, &additive };  
    vector<string> names = { "raw", "anti aliased", "additive" };

    for ( int i=0; i<3; i++ ) {

        for ( int j=0; j<3; j++ ) {

            synth.disconnect(*oscs[j],"signal",audio,"left")
                 .disconnect(*oscs[j],"signal",audio,"right");

            synth.run_again(SAMPLE_RATE/2);

            synth.connect(*oscs[(j+1)%3],"signal",audio,"left")
                 .connect(*oscs[(j+1)%3],"signal",audio,"right");

            std::cout << names[(j+1)%3] << "\n";
            synth.run_again(SAMPLE_RATE);

        }

    }

    return 0; 

}
 
