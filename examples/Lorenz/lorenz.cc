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

#define rho 28
#define sigma 10
#define beta (8/3)

class Lorenz : public Oscillator {

public:

    Lorenz() {
        alt_signal = add_output("alt_signal");
    }

    void init() {
        x = .1;
        y = .2;
        z = .3;
    }

    void update() {

        Oscillator::update();

        for ( int i=0; i<100; i++ ) {

            f1 = sigma * ( y - x );
            f2 = x * ( rho - z ) - y;
            f3 = x * y - beta * z;

            double q = (inputs[frequency] + inputs[modulation]) * TS / 100;

            x += q*f1;
            y += q*f2;
            z += q*f3;

        }

        outputs[signal] = inputs[amplitude] * y;
        outputs[alt_signal] = inputs[amplitude] * z;
    }

private:

    double x, y, z,
        f1, f2, f3;

    int alt_signal;

};

int main(int argc, char * argv[]) {

    Lorenz system, lfo;
    Audio audio;
    Controls controls;
    Container synth;

    system.set_input("frequency", 500);
    system.set_input("amplitude", 0.01);

    lfo.set_input("frequency", 2);
    lfo.set_input("amplitude", 5);
    
    synth.add(system)
         .add(audio)
         .add(controls)
         .add(lfo)
         .propagate_to(controls)
         .connect(system,"signal",audio,"left")
         .connect(system,"alt_signal",audio,"right")
         .connect(lfo, "signal", system, "modulation");

    // controls.control(system, "frequency", 50, 500, 18);

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}
 