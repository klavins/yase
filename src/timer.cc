// 
// YASE Timer Module Implementation
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

#include "timer.hh"
#include "yase.hh"

namespace yase {


  Timer::Timer() : Module(), active(false) {

  }

  //! Set the duration of a timer and define what to do when the
  //! time is up.
  //! \param t The duration, in seconds
  //! \param f A method, for example expressed as a lambda
  void Timer::set(float t, function<void()> f) {
    duration = t;
    handler = f;
    active = true;
    timer = 0;
  }

  //! Reset the timer. Calling set on a timer only triggers its method once.
  //! If you want it to trigger again (with the same associated method) cal reset.
  void Timer::reset() {
    timer = 0;
    active = true;
  }

  void Timer::reset(float t) {
    duration = t;
    reset();
  }  

  void Timer::init() {

  }

  void Timer::update() {

    timer += ts;

    if ( active && timer > duration ) {
        active = false;      
        handler();
    }

  }    

}
