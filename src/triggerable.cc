// 
// YASE Triggerable Module Implementation
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

#include "triggerable.hh"
#include "yase.hh"

namespace yase {

  Triggerable::Triggerable() : Module() {
      external_trigger = add_input("trigger");
      set_input(external_trigger,0);
  }

  void Triggerable::init() {
      trigger_memory = 0.0;
  }

  void Triggerable::update() {

    if ( trigger_memory == 0.0 && inputs[external_trigger] > 0.9 ) {
        trigger_memory = 1.0;
        trigger();
    } else if ( trigger_memory == 1.0 && inputs[external_trigger] < 0.1 ) {
        trigger_memory = 0.0;
        release();
    }    

  }    

}

