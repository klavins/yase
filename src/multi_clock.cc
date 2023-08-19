// 
// YASE MultiClock Module Implementation
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

#include "multi_clock.hh"
#include "yase.hh"

namespace yase {

  MultiClock::MultiClock() : denominators({ 1, 2, 3, 4, 6, 8, 12, 16, 32 }) {

    for ( int i=0; i<9; i++ ) {
      df[i] = add_input("gate_1/" + std::to_string(denominators[i]));
      set_input(df[i], 0.5);
      clock[i] = add_output("clock_1/" + std::to_string(denominators[i]));
    }

    rate = add_input("rate");
    set_input("rate", 60);

  }

  void MultiClock::init() {
    phase = 0.0;
  }

  void MultiClock::update() {
    period = 60.0 / inputs[rate];
    phase += TS * inputs[rate] / 60.0;
    if ( phase >= 1.0 ) {
      phase -= 1.0;
    }
    for ( int i=0; i<9; i++ ) {
      phases[i] = fmod ( phase * denominators[i], 1.0 );
      outputs[clock[i]] = phases[i] <= inputs[df[i]] ? 1.0 : -1.0;    
    }
  }

}

