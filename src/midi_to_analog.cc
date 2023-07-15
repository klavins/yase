// 
// YASE MidiToAnalog Module Implementation
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

#include "midi_to_analog.hh"
#include "yase.hh"

namespace yase {

  MidiToAnalog::MidiToAnalog(string device_name) : Container(), midi_input(device_name) {

      add(midi_input);

      frequency = add_output("frequency");
      velocity = add_output("velocity");
      aftertouch = add_output("aftertouch");
      trigger = add_output("trigger");

      outputs[frequency] = 0.0;
      outputs[trigger] = 0.0;
      outputs[velocity] = 0.0;
      outputs[aftertouch] = 0.0;

      listen(MIDI_KEYDOWN, [&] (const Event &e) {
          std::cout << "down: " << e.code << "," << e.id << "," << e.value << "\n";
          outputs[frequency] = e.frequency();
          outputs[velocity] = e.value / 127.0;
          outputs[trigger] = 1.0;
      });

      listen(MIDI_KEYUP, [&] (const Event &e) {
          std::cout << "up\n"; 
          outputs[trigger] = 0.0;
      });

      listen(MIDI_AFTER_TOUCH, [&] (const Event &e) {
          std::cout << "aftertouch: " << e.code << "," << e.id << "," << e.value << "\n";
          outputs[aftertouch] = e.id / 127.0;
      });

  }

  void MidiToAnalog::init() {

    Container::init();

  }

  void MidiToAnalog::update() {

    Container::update();

  }    

}
