// 
// YASE Triangle Module Implementation
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

  Triangle::Triangle() : Oscillator() {
    update_fcn = &Triangle::additive;
  }

  void Triangle::init() {
    Oscillator::init();
  }

  void Triangle::update() {
    Oscillator::update();
    CALL_MEMBER_FN(this, update_fcn);
    outputs[signal] *= inputs[amplitude];
  }    

  void Triangle::set_type(std::string name) {

    if ( name == "raw" )  {
      update_fcn = &Triangle::raw;
    } else if ( name == "additive" ) {
      update_fcn = &Triangle::additive;
    } else if ( name == "ptr1" ) {
      update_fcn = &Triangle::ptr1;
    } else {
      update_fcn = &Triangle::ptr1;
    }

  }    

  void Triangle::raw() {

  }    

  void Triangle::ptr1() {

  }    

  void Triangle::additive() {
   
    outputs[signal] = 0;
    int n = 1;
    while ( n * inputs[frequency] < SAMPLE_RATE / 2 ) {
      outputs[signal] += sin(n*2*M_PI*phase + inputs[modulation])/(n*n);
      n += 2;
    }
    outputs[signal] *= 8/(M_PI*M_PI);

  }    

}

