// 
// YASE Mix Module Implementation
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

#include "mix.hh"
#include "yase.hh"

namespace yase {

  Mix::Mix(std::vector<std::tuple<Module&, string, double, double>> connections) 
     : Container(), 
       mix_left(connections.size()),
       mix_right(connections.size())  {

    int i=0;
    std::string s = "signal_",
                g = "gain_";

    for ( auto [module, signal, left, right] : connections ) {
      connect(module, signal, mix_left, s + std::to_string(i));
      connect(module, signal, mix_right, s + std::to_string(i));
      mix_left.set_input(g+std::to_string(i), left);
      mix_right.set_input(g+std::to_string(i), right);
      i++;
    }

    attach_outputs({
      {"left", mix_left, "signal"},
      {"right", mix_right, "signal"}
    });

  }

  void Mix::init() {
    Container::init();
  }

  void Mix::update() {
    Container::update();
  }

}

