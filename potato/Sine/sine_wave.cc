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

    Saw raw_osc("raw");
    AntiAlias osc(raw_osc);
    Gain gain;
    Audio audio;
    Envelope env;
    Biquad filter;
    Timer timer;
    Transform freq_scale( [] (double u) { return 5000; });
    Transform res_scale ( [] (double u) { return 10.0; });
    Transform invert ( [] (double u) { return -u; });
    Container synth;

    osc.set_input("frequency", 55);
    gain.set_input("amplitude", 0.25);

    synth.connect(freq_scale, "signal", filter, "frequency")
         .connect(res_scale,  "signal", filter, "resonance")
         .path(osc, filter, gain, invert)
         .connect(env, "signal", osc, "amplitude")
         .connect(gain, "signal", audio, "left")
         .connect(invert, "signal", audio, "right")
         .add(timer);

    env.set_adsr(0.5, 1.0, 1.0, 0.1);

    timer.set(0.1, [&] {
      env.trigger();
    });

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}
 
