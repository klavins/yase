// 
// YASE ExpDecay Module Implementation
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

#include "exp_decay.hh"
#include "yase.hh"

namespace yase {

  ExpDecay::ExpDecay() {

    rate = add_input("rate");
    from = add_input("from");
    to = add_input("to");

    inputs[rate] = 1;
    inputs[from] = 1;
    inputs[to] = 0;

    signal = add_output("signal");
    outputs[signal] = to;

  }

  void ExpDecay::init() {
    value = 0.0;
  }

  //! Reset the envelope to output the value specified by the input "from"
  void ExpDecay::trigger() {
    value = 1.0;
  }

  void ExpDecay::update() {

    //std::cout << inputs[rate] << ", " << inputs[from] << ", " <<  inputs[to] << " ==> ";

    value -= TS * inputs[rate] * value;
    outputs[signal] = inputs[from] * value + inputs[to] * (1-value);

    //std::cout << outputs[signal] << "\n";

  }

}
