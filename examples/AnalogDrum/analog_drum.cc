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

    Square raw_osc;
    AntiAlias osc(raw_osc);
    Gain gain;
    Audio audio;

    Biquad filter;
    Transform filter_freq( [] (double u) { return 200; });  // 100 to 10000 
    Transform filter_res ( [] (double u) { return 10; });   // 1 to 15

    ExpDecay freq;
    Transform freq_rate ( [] (double u) { return 10; });     // 0.1 to 20
    Transform freq_from ( [] (double u) { return 50; });     // 40 to 100
    Transform freq_to   ( [] (double u) { return 55; });     // 40 to 100

    ExpDecay amp;
    amp.set_input("rate", 1);
    amp.set_input("from", 1);
    amp.set_input("to", 0);    
    Transform amp_rate( [] (double u) { return 10; });       // 0.1 to 20

    Timer timer;
    Container synth;

    gain.set_input("amplitude", 0.5);

    synth.connect(filter_freq, "signal", filter, "frequency")
         .connect(filter_res,  "signal", filter, "resonance")

         .connect(freq_rate, "signal", freq, "rate")
         .connect(freq_from, "signal", freq, "from")
         .connect(freq_to,   "signal", freq, "to")

         .connect(amp_rate,  "signal", amp, "rate")

         .connect(freq, "signal", osc, "frequency")
         .connect(amp,  "signal", osc, "amplitude")

         .path(osc, filter, gain)
         .connect(gain, "signal", audio, "left")
         .connect(gain, "signal", audio, "right")

         .add(timer);

    DEBUG

    timer.set(1, [&]() {
        DEBUG
        freq.trigger();
        amp.trigger();
        timer.reset();
    });

    DEBUG

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}
