// 
// YASE Square Module Implementation
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

namespace yase {

  void Square::update() {

    Oscillator::update();
    
    if ( phase < 0.5 ) {
        outputs[signal] = 1;
    } else {
        outputs[signal] = -1;
    }
    
  }      

  // void Square::update() {

  //   Oscillator::update();

  //   outputs[signal] = 0;
  //   int n = 1;
  //   while ( n * inputs[frequency] < 0.5 * SAMPLE_RATE ) {
  //     outputs[signal] += sin(n*2*M_PI*phase  + inputs[modulation])/n;
  //     n += 2;
  //   }
  //   outputs[signal] *= inputs[amplitude] * 4 / M_PI;
  // }    

}

