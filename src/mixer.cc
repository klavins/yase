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

