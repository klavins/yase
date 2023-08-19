// 
// YASE Delay Module Implementation
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

#include "delay.hh"
#include "yase.hh"

namespace yase {

  //! Construct a new delay with the given duration (in steps)
  //! \param duration The number of steps
  Delay::Delay(int duration) : Triggerable(), duration(duration) {

    in_signal = add_input("signal");
    frequency = add_input("frequency");
    out_signal = add_output("signal");
    /* trigger(); */

  }

  Delay::Delay() : Delay(1) {}

  //! Set the desired delay. 
  //! \param new_duration
  void Delay::set(int new_duration) {
    if ( new_duration > duration ) {
      // Nothing to do, just let the buffer grow
    } else if ( new_duration < duration ) {
      // Resize 
      while ( buffer.size() > new_duration ) {
        buffer.pop_back();
      }
    }
    duration = new_duration;
  }

  //! Clear the delay buffer.
  void Delay::clear() {
    buffer.clear();
  }

  void Delay::trigger() {
      
      if ( inputs[frequency] > 0 ) {          
          set(SAMPLE_RATE/inputs[frequency]);
          clear();
      }
      
  }

  //! Check if the buffer is full
  //! \return True if the buffer size equals the duration
  bool Delay::is_full() {
    return buffer.size() >= duration;
  }

  void Delay::init() {
   Triggerable::init(); 
  }

  void Delay::update() {

    Triggerable::update();

    buffer.push_front(inputs[in_signal]);

    if ( buffer.size() > duration) {
        outputs[out_signal] = buffer.back();
        buffer.pop_back();
    }

    while ( buffer.size() > duration ) { // just in case
        buffer.pop_back();
    }       

  }    

}

