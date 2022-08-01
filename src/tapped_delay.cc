// 
// YASE TappedDelay Module Implementation
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

#include "tapped_delay.hh"
#include "yase.hh"

namespace yase {

  TappedDelay::TappedDelay(double duration) : Module(), duration(duration), buffer_index(0) {
    duration_in_samples = ceil(duration * SAMPLE_RATE);
    buffer = new double[duration_in_samples];
    signal = add_input("signal");
  }

  //! Add a new tap and return its name. The position of the tap can then set using the
  //! name. For example
  //! \code
  //! auto tap = delay.add_tap();
  //! delay.set_input(tap, 0.5);
  //! \endcode
  //! \return The name of the tap
  string TappedDelay::add_tap() {
    int index = inputs.size();
    string tap_name = "tap_" + std::to_string(index);
    add_input(tap_name);
    add_output(tap_name);
    return tap_name;
  }

  void TappedDelay::init() {
    for(int i=0;i<duration_in_samples;i++) {
      buffer[i] = 0.0;
    }
  }

  void TappedDelay::update() {
    add_sample(inputs[signal]);
    for ( int i=0; i<inputs.size()-1; i++ ) {
      outputs[i] = get_value(tap_position(i));
    }
  }

  double TappedDelay::tap_position(int i) {
    return inputs[i+1];
  }

  void TappedDelay::add_sample(double u) {
    buffer[buffer_index] = u;
    buffer_index++;
    if ( buffer_index > duration_in_samples ) {
      buffer_index = 0;
    }
  }

  int TappedDelay::fix(int n) {
    int m = n;
    while ( m < 0 ) {
      m += duration_in_samples;
    }
    while ( m >= duration_in_samples ) {
      m -= duration_in_samples;
    }
    return m;    
  }

  double TappedDelay::get_value(double position) {
    int a = fix(buffer_index + floor(position * SAMPLE_RATE)),
        b = fix(buffer_index + ceil(position * SAMPLE_RATE ));
    double A = ( a - buffer_index ) / SAMPLE_RATE;
    return (buffer[b] - buffer[a])*(position-A) + buffer[a];
  }

  TappedDelay::~TappedDelay() {
    delete[] buffer;
  }  

}

