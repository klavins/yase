// 
// YASE Example
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

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "yase.hh"
#include "mono.hh"

using namespace yase;
using namespace nlohmann;

int main(int argc, char * argv[]) {

     json config = get_config("./examples/Mono/akai.json"); 

     std::cout << config << "\n";

     Container synth;
     Audio audio;
     MidiInput keyboard(config["keyboard"]),
               controller(config["controller"]);

     Mono mono(keyboard, controller, config);

     synth.add(mono)
          .add(audio)
          .add(keyboard)
          .add(controller)
          .propagate_to(mono);

     synth.connect(mono, "signal", audio, "left")
          .connect(mono, "signal", audio, "right");

     // Go!
     synth.run(UNTIL_INTERRUPTED);

     // Shutdown
     mono.clear_leds();
     return 0;

}