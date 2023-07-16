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

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine;
    SampleAndHold sample_and_hold;
    Noise noise;
    Audio audio;
    Transform scale([](double u) { return 100*u + 300;});
    Square lfo;
    Container synth;

    lfo.set_input("frequency", 10);
    
    synth.path(noise,sample_and_hold,scale)
         .connect(lfo, "signal", sample_and_hold, "trigger")
         .connect(scale, "signal", sine, "frequency")
         .connect(sine,"signal",audio,"left")
         .connect(sine,"signal",audio,"right");

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}