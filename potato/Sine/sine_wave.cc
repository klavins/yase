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

#include "yase.hh"
#include "../potato_audio.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine1, sine2;
    PotatoAudio audio;
    Container synth;

    DEBUG
    sine1.set_input("frequency", 440);
    sine2.set_input("frequency", 441);
    DEBUG
    synth.connect(sine1,"signal",audio,"left")
         .connect(sine2,"signal",audio,"right");
    DEBUG
    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}
 
