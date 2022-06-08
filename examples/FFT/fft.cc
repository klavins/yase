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

    double freq = B6;
    double cycles = 31;

    Container synth;
    Audio audio;
    Saw osc;
    Cycle cycle;
    AntiAlias aa_osc(osc);
    FFT fft(cycles*SAMPLE_RATE / freq);
    Timer timer;
    
    osc.set_type("raw");

    cycle.set({ G4, C5, A4, F4, D4, G4, E4, C4, G5, C6, A5, F5, A5, B5, C6 }, [&] (double freq) {
      osc.set_input("frequency", freq);
    }, 2.0);        
   
    synth.add(timer)
         .add(cycle);
    synth.connect(aa_osc, fft);
    synth.connect(aa_osc, "signal", audio, "left");
    synth.connect(aa_osc, "signal", audio, "right");

    timer.set(0.025,[&]() {
        std::cout << fft.json() << "\n";
        timer.reset();
    });
    
    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}