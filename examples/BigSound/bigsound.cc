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

    Container synth;
    Audio audio;
    Biquad filter;
    Saw raw_saw;
    AntiAlias saw(raw_saw);
    Envelope wave_envelope, filter_envelope;
    Cycle cycle;
    Gain gain;
    Transform scale( [] (double u) { return 500 + 4000*u; }),
              invert( [] (double u) { return -u; });
    Echo echo;

    raw_saw.set_type("raw");
    raw_saw.set_input("amplitude", 0.4);

    filter.set_input("resonance", 10);

    echo.set_input("duration", SAMPLE_RATE/2);
    echo.set_input("amplitude", 0.8);

    wave_envelope.set_input("attack",  0.8);
    wave_envelope.set_input("decay",   1.0);
    wave_envelope.set_input("sustain", 0.5);
    wave_envelope.set_input("release", 0.1);

    filter_envelope.set_input("attack",  2.0);
    filter_envelope.set_input("decay",   2.0);
    filter_envelope.set_input("sustain", 0.0);
    filter_envelope.set_input("release", 0.1);    

    synth.path(saw, filter, gain, echo)
         .connect(echo, invert)
         .connect(echo, "signal", audio, "left")
         .connect(invert, "signal", audio, "right")
         .connect(wave_envelope, "signal", gain, "amplitude")
         .connect(filter_envelope, scale)
         .connect(scale, "signal", filter, "frequency")
         .add(cycle);

    cycle.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
      raw_saw.set_input("frequency", freq);
      wave_envelope.trigger();
      filter_envelope.trigger();
    }, 3.0);   

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}