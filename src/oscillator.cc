// 
// YASE Oscillator Module Implementation
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

#include <math.h>
#include "yase.hh"

namespace yase {

  Oscillator::Oscillator() {
      frequency = add_input("frequency");
      amplitude = add_input("amplitude");
      modulation = add_input("modulation");
      tuning = add_input("tuning");
      harmonic = add_input("harmonic");
      signal = add_output("signal");
      inputs[amplitude] = 1;
      inputs[frequency] = 440;
      inputs[modulation] = 0;
      inputs[tuning] = 0;
      inputs[harmonic] = 0;
  }

  void Oscillator::init() {
      phase = 0;
  }

  void Oscillator::update() {
      double h = pow(2, floor(inputs[harmonic]));
      phase += ts * (h*inputs[frequency] + CENT*floor(2*inputs[tuning])/2);
      if ( phase > 1.0 ) {
        phase -= 1.0;
      }     
  }

}

