/*! \file */

// 
// YASE Echo Module Implementation
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

#include "echo.hh"
#include "yase.hh"

namespace yase { 

  Echo::Echo() : Container(), sum(2) {

    signal = add_input("signal");
    signal = add_output("signal");
    amplitude = add_input("amplitude");
    duration = add_input("duration");

    equate_input("signal", sum, "signal_0");
    equate_input("duration", delay, "duration");
    equate_input("amplitude", gain, "amplitude");

    equate_output("signal", sum, "signal");

    connect(gain, "signal", sum, 1);
    connect(sum, "signal", delay, "signal");
    connect(delay, "signal", gain, "signal");

  }

  void Echo::init() {
    Container::init();
  }

  void Echo::update() {
    Container::update();
  }    

}

