#include "gain.hh"
#include "yase.hh"

namespace yase {

  Gain::Gain() {
    signal_in = add_input("signal_in");
    amplitude = add_input("amplitude");
    signal_out = add_output("signal_out");
  }

  void Gain::init() {

  }

  void Gain::update() {
    outputs[signal_out] = inputs[amplitude] * inputs[signal_in];
  }    

}

