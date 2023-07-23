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

    Sine sine1, sine2;
    Envelope e1, e2;
    MultiClock clock;
    Audio audio;
    Container synth;
    
    sine1.set_input("frequency", A4);
    sine2.set_input("frequency", B4);

    e1.set_adsr(0.01, 1.0, 1.0, 0.01);
    e2.set_adsr(0.01, 1.0, 1.0, 0.01);

    clock.set_input("gate_1/2", 0.1 );
    clock.set_input("gate_1/3", 0.1 );

    synth
         .connect(clock, "clock_1/2", e1, "trigger")
         .connect(clock, "clock_1/3", e2, "trigger")
         .connect(e1, "signal", sine1, "amplitude")
         .connect(e2, "signal", sine2, "amplitude")
         .connect(sine1,"signal",audio,"left")
         .connect(sine2,"signal",audio,"right");

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}