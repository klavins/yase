// 
// YASE Oscillator Group Module Implementation
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

  OscGroup::OscGroup() : Oscillator() {

    oscillators.push_back(new Sine);
    oscillators.push_back(new Saw);
    oscillators.push_back(new Triangle);
    oscillators.push_back(new Square);
    oscillators.push_back(new Noise);

    current = 0;

  }

  void OscGroup::update() {

    oscillators[current]->copy_inputs(*this);
    oscillators[current]->update();
    oscillators[current]->copy_outputs(*this);

  }    

  void OscGroup::select(float x) {

    int i = floor(x * oscillators.size());

    if ( i >= 0 && i < oscillators.size() ) {
      current = i;
    }

  }

  OscGroup::~OscGroup() {
    for (auto o : oscillators) {
       delete o;
     } 
     oscillators.clear();
  }

}