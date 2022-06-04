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

class String : public Container {

public:
  
  String() : filter({2.01}, {1, 1}), aa_impulse(impulse) {

    frequency = add_input("frequency");
    add_output("signal"); 
    impulse.set_type("square");

    path(sum, delay, filter);
    connect(filter, "signal", sum, 0);
    connect(aa_impulse, "signal", sum, 1);
    equate_output("signal", delay, "signal");

  }

  double length() {
    return ((int) (SAMPLE_RATE/inputs[frequency])) - 3;
  }

  void pluck() {
    delay.clear();
    delay.set(length());
    impulse.set(100); // constant or variable? Seems like the area
                      // where you pluck the string is independent
                      // of pitch
    impulse.trigger();
  }

private:

  Delay delay;
  IIRFilter filter;
  Impulse impulse;
  AntiAlias aa_impulse;
  Sum sum;
  int frequency;

};

int main(int argc, char * argv[]) {

    String string;
    Audio audio;
    Cycle cycle;    
    Container synth;
    Delay delay(0.005 * SAMPLE_RATE); // To make a more stereo sound

    cycle.set({ G4, C5, A4, F4, D4, G4, E4, C4 }, [&] (double freq) {
      string.set_input("frequency", freq);
      string.pluck();
    }, 1.0);    

    synth.connect(string,"signal",audio,"right")
         .connect(string,"signal",delay,"signal")
         .connect(delay,"signal",audio,"left")
         .add(cycle);

    synth.run(8*SAMPLE_RATE);
    return 0; 

}
 