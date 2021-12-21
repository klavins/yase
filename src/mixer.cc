#include <string>
#include "mixer.hh"
#include "yase.hh"

namespace yase {

  Mixer::Mixer(int n) : n(n) {

    for ( int i=0; i<n; i++) {
      add_input("signal_" + std::to_string(i));
    }

    for ( int i=0; i<n; i++ ) {
      add_input("gain_" + std::to_string(i));
      inputs[n+i] = 1; // default gain
    }

    signal = add_output("signal");

  }

  void Mixer::init() {

  }

  void Mixer::update() {
    outputs[signal] = 0;
    for ( int i=0; i<n; i++ ) 
      outputs[signal] += inputs[n+i] * inputs[i];
  }    

}

