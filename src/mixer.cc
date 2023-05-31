// 
// YASE Mixer Module Implementation
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

#include <string>
#include "mixer.hh"
#include "yase.hh"

namespace yase {

  Mixer::Mixer(int n) : n(n) {

    // Note: the order in which the inputs below are added matters, because
    // it determines the index. In particular, the output_gain is added last. 

    for ( int i=0; i<n; i++) {
      add_input("signal_" + std::to_string(i));
    }

    for ( int i=0; i<n; i++ ) {
      add_input("gain_" + std::to_string(i));
      inputs[n+i] = 1; // default gain
    }

    output_gain = add_input("output_gain");
    inputs[output_gain] = 1;
    signal = add_output("signal");

  }

  void Mixer::init() {}

  void Mixer::update() {
    outputs[signal] = 0;
    for ( int i=0; i<n; i++ ) 
      outputs[signal] += inputs[n+i] * inputs[i];
    outputs[signal] *= inputs[output_gain];
  }    

  double Mixer::get_amplitude_input(int i) {
    return get_input(i+n);
  }  

  void Mixer::set_amplitude_input(int i, double value) {
    set_input(i+n, value);
  }

}

