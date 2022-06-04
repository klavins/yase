// 
// YASE Fader Module Implementation
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
#include <stdlib.h>
#include "fader.hh"
#include "yase.hh"

namespace yase {

  //! Construct a new fader. The default response is linear. And the default tracking gain is TRACKING_GAIN.
  //! \param min The minimum value of the fader
  //! \param max The maximum value of the fader
  Fader::Fader(double min, double max) : min_val(min), max_val(max), is_linear(true), tracking_gain(FADER_GAIN) {
    target = add_input("target");
    value = add_output("value");
    set_input(target,0); 
  }

  //! Construct a new fader with range [0,1]. The default response is linear. And the default tracking gain is TRACKING_GAIN.
  Fader::Fader() : min_val(0), max_val(1), is_linear(true) {
    target = add_input("target");
    value = add_output("value");
    set_input(target,0); 
  }

  void Fader::init() {
    outputs[value] = adjusted_target();
  }

  double Fader::adjusted_target() {
    double v = inputs[target];
    if ( is_linear ) {
      return min_val + ( max_val-min_val ) * v / 127.0;
    } else {
      double temp = pow(base,v/127.0);
      return (max_val-min_val)*(temp-1)/(base-1)+min_val;      
    }
  }

  void Fader::update() {
    outputs[value] -= ts * tracking_gain * (outputs[value] - adjusted_target());
  }    

  //! Set the target of the fader to a random value from within its min and max values. 
  void Fader::randomize() {
    set_input(target, rand() % 127 );
  }

}
