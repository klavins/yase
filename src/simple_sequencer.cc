// 
// YASE SimpleSequencer Module Implementation
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

#include "simple_sequencer.hh"
#include "yase.hh"

namespace yase {

  SimpleSequencer::SimpleSequencer(int n) : num_steps(n) {

    for (int i=0; i<num_steps; i++) {
      add_input("v_" + std::to_string(i));
    }

    trigger = add_input("trigger");
    signal = add_output("signal");

  }

  void SimpleSequencer::init() {

    step = 0;
    trigger_state = 0;

  }

  void SimpleSequencer::update() {

    if ( trigger_state == 0 && inputs[trigger] > 0.9 ) {
      trigger_state = 1;
      step = (step + 1)%num_steps;

    } else if ( trigger_state == 1 && inputs[trigger] < 0.1 ) {
      trigger_state = 0;
    }

    outputs[signal] = inputs[step];

  }    

}
