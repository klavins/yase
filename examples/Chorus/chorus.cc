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

    double C = 0.001,    // tap center (seconds)
           A = 0.0001,   // tap position amplitude (seconds)
           F = 1.5;      // frequency (per second) 
    
    Container synth;
    Audio audio;
    Saw raw_saw;
    Sine lfo;
    Transform scale([&](double u) { return A*u + C; });
    AntiAlias saw(raw_saw);
    TappedDelay delay(2*C);
    Envelope wave_envelope;
    Player player;
    Gain gain;
    Sum sum;

    auto [position,tap] = delay.add_tap();
    delay.set_input(position, C);

    lfo.set_input("frequency", F);

    saw.set_input("amplitude", 0.4);
    gain.set_input("amplitude", 0.35);
    wave_envelope.set_adsr(0.005, 1.0, 1.0, 0.1);

    synth.connect(saw, delay)
         .connect(saw, "signal", sum, 0)
         .connect(delay, tap, sum, 1)
         .connect(sum, gain)
         .connect(gain, "signal", audio, "left")
         .connect(gain, "signal", audio, "right")
         .connect(wave_envelope, "signal", saw, "amplitude")
         .connect(lfo, scale)
         .connect(scale, "signal", delay, position)
         .add(player);

    player.set({ C2,  REST, C2,  REST, AS1, REST, AS1, REST, 
                 GS1, REST, GS1, REST, G1,  REST, G1,  REST }, 
      [&] (double freq) {
        if ( freq != REST ) {
          saw.set_input("frequency", freq);
          wave_envelope.trigger();
        } else {
          wave_envelope.release();
        }
      }, 1.5);

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}