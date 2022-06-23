// 
// YASE AntiAlias Module Implementation
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

#include "anti_alias.hh"
#include "yase.hh"

namespace yase {

  AntiAlias::AntiAlias(Module &m) : module(&m) {

    m.set_ts(ts/2);

    lowpass.set_ts(ts/2);
    lowpass.set_input("frequency", 0.5 * SAMPLE_RATE);
    lowpass.set_input("resonance", 0.75);

    for (int i=0; i<module->num_inputs(); i++) {
      add_input(module->get_input_name(i));
      set_input(i,module->get_input(i));
    }
    
    signal = add_output("signal");

    lowpass_signal_in = lowpass.get_input_index("signal");
    lowpass_signal_out = lowpass.get_output_index("signal");

    try {
      module_signal_out = module->get_output_index("signal");
    } catch ( Exception &e ) {
      throw Exception("AntiAlias expected a child module with an output named 'signal");
    }

  }

  void AntiAlias::init() {}

  void AntiAlias::set(double frequency, double resonance) {
    lowpass.set_input("frequency", frequency);
    lowpass.set_input("resonance", resonance);
  }

  void AntiAlias::update() {

    for ( int i=0; i<inputs.size(); i++ ) {
      module->set_input(i, inputs[i]);
    }

    module->update();
    lowpass.set_input(lowpass_signal_in, module->get_output(module_signal_out));
    lowpass.update();

    module->update();
    lowpass.set_input(lowpass_signal_in, module->get_output(module_signal_out));
    lowpass.update();

    outputs[signal] = lowpass.get_output(lowpass_signal_out);

  }    

}

