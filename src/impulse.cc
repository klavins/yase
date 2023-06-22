// 
// YASE Impulse Module Implementation
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

#include "impulse.hh"
#include "yase.hh"
#include <stdlib.h> 

namespace yase {

  Impulse::Impulse() : length(1) {
    signal = add_output("signal");
    update_fcn = &Impulse::square;
  }

  void Impulse::init() {}

  void Impulse::update() {
    CALL_MEMBER_FN(this, update_fcn);
  }

  //! Set the type of the impulse.
  //! \param type Either "square" or "random"
  void Impulse::set_type(std::string type) {
    if ( type == "random" ) {
      update_fcn = &Impulse::random;
    } else {
      update_fcn = &Impulse::square;
    }
  }

  void Impulse::square() {
    if ( n++ < length ) {
      outputs[signal] = 1;
    } else {
      outputs[signal] = 0;
    }
  }    

  void Impulse::random() {
    if ( n++ < length ) {
      outputs[signal] = 0.001 * (rand()%1000);
    } else {
      outputs[signal] = 0;
    }
  }    

  //! Trigger the impulse
  void Impulse::trigger() {
    n = 0;
  }

  //! Set the number of time steps for the impulse to be non-zero.
  //! Each time step is 1 / SAMPLE_RATE seconds long. 
  //! \param k The number of time steps
  void Impulse::set(int k) {
    length = k;
  }  

}

