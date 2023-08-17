// 
// YASE SampleAndHold Module Implementation
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

  SampleAndHold::SampleAndHold() : Triggerable() {
    signal = add_input("signal");
    signal = add_output("signal");
  }

  void SampleAndHold::init() {
      Triggerable::init();
      inputs[signal] = 0;
  }

  void SampleAndHold::update() {
      Triggerable::update();
  }    

  void SampleAndHold::trigger() {
    outputs[signal] = inputs[signal];
  }

  void SampleAndHold::release() {}

}

