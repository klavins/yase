// 
// YASE Saw Module Implementation
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

  Saw::Saw() : Oscillator() {
    update_fcn = &Saw::additive;
  }  

  void Saw::init() {
    Oscillator::init();
  }

  void Saw::update() {
    Oscillator::update();
    CALL_MEMBER_FN(this, update_fcn);
    outputs[signal] *= inputs[amplitude];
  }    

  void Saw::set_type(std::string name) {

    if ( name == "raw" )  {
      update_fcn = &Saw::raw;
    } else if ( name == "additive" ) {
      update_fcn = &Saw::additive;
    } else if ( name == "ptr1" ) {
      update_fcn = &Saw::ptr1;
    } else {
      update_fcn = &Saw::ptr1;
    }

  }    

  void Saw::raw() {
      outputs[signal] = inputs[amplitude] * ( 2 * phase - 1 );
  }    

  void Saw::ptr1() {
    double h = 1,
           P0 = SAMPLE_RATE/inputs[frequency],  // 44100 / 440 = 100
           T0 = 1/P0,                           // 0.1
           cdc = T0,                            // 0.1
           DC = 1 + cdc,                        // 1.1
           a1 = 2 - 2*h*P0,                     // 2 - 2*100 = -198
           a0 = 2*h - DC;                       // 1 
    if ( phase >= T0 ) {
        outputs[signal] = 2*phase - DC;
    } else {
        outputs[signal] = a1*phase + a0;
    }
  }    

  void Saw::additive() {
    outputs[signal] = 0;
    int n = 1;
    while ( inputs[frequency] * n < SAMPLE_RATE / 2 ) {
      outputs[signal] += sin(n*2*M_PI*phase + inputs[modulation])/n;
      n++;
    }
  }    

}
