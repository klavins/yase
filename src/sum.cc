// 
// YASE Sum Module Implementation
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

#include <string>
#include "sum.hh"
#include "yase.hh"

namespace yase {

  //! Construct a Sum Module with n inputs
  //! \param n The number of inputs

  Sum::Sum(int n) : size(n) {

    for (int i=0; i<size; i++ ) {
      std::string name = "signal_" + std::to_string(i);
      add_input(name);
    }

    signal = add_output("signal");

  }

  Sum::Sum() : Sum(2) {}

  void Sum::init() {

  }

  void Sum::update() {
    outputs[signal] = 0;
    for ( int i=0; i<size; i++ ) 
      outputs[signal] += inputs[i];
  }    

}

