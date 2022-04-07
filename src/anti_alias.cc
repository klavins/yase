#include "anti_alias.hh"
#include "yase.hh"

namespace yase {

  AntiAlias::AntiAlias(Module &m) : module(&m) {

    m.set_ts(ts/2);

    lowpass.set_ts(ts/2);
    lowpass.set_input("frequency", 0.5 * SAMPLE_RATE);
    lowpass.set_input("resonance", 0.75);

    signal = add_input("signal");
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

    module->update();
    lowpass.set_input(lowpass_signal_in, module->get_output(module_signal_out));
    lowpass.update();

    module->update();
    lowpass.set_input(lowpass_signal_in, module->get_output(module_signal_out));
    lowpass.update();

    outputs[signal] = lowpass.get_output(lowpass_signal_out);

  }    

}

