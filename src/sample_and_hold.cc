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

#include "sample_and_hold.hh"
#include "yase.hh"

namespace yase {

  SampleAndHold::SampleAndHold() {
    signal = add_input("signal");
    signal = add_output("signal");
    trigger = add_input("trigger");
  }

  void SampleAndHold::init() {
      trigger_state = 0;
      inputs[signal] = 0;
  }

  void SampleAndHold::update() {
      if (trigger_state == 0 && inputs[trigger] > 0.9 ) {
        trigger_state = 1;
        outputs[signal] = inputs[signal];
      } else if ( trigger_state == 1 && inputs[trigger] < 0.1 ) {
        trigger_state = 0;
      }
  }    

}

