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

    Sine sines[10];
    Mixer mixer(10);
    Audio audio;
    Container synth;

    synth.set_thread_number(11);

    for ( int i=0; i<10; i++ )  {
      sines[i].set_input("frequency", 440);
      synth.add(sines[i], 0);
      synth.connect(sines[i], "signal", mixer, i);
     }

    synth.add(audio,0)
         .add(mixer,0)
         .connect(mixer,"signal",audio,"left")
         .connect(mixer,"signal",audio,"right");

    synth.run_threaded(2*SAMPLE_RATE);

    return 0; 

}

 