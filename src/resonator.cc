// 
// YASE Resonator Module Implementation
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
#include "resonator.hh"
#include "yase.hh"

namespace yase {

  Resonator::Resonator() : IIRFilter (3,3) {

    theta = add_input("theta");
    radius = add_input("radius");
    inputs[theta] = 1;
    inputs[radius] = 0.5;

  }

  void Resonator::init() {

  }

  void Resonator::update() {

    b[0] = 1;
    b[1] = 0;
    b[2] = -inputs[radius];

    a[0] = 1;
    a[1] = -2 * inputs[radius] * cos(inputs[theta]);
    a[2] = inputs[radius] * inputs[radius];

    IIRFilter::update();

  } 

}

