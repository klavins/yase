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

#include <math.h>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    double freq = 1975; // B6
    double cycles = 10;

    Saw osc;
    AntiAlias aa_osc(osc);
    osc.set_type("raw");
    osc.set_input("frequency", freq);
    FFT fft(cycles*SAMPLE_RATE / freq);

    Container synth;
    synth.connect(aa_osc,fft);
    synth.run(cycles*SAMPLE_RATE/freq+1);

    fft.html();

    return 0; 

}