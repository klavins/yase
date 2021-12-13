#include "gain.hh"
#include "yase.hh"

namespace yase {

  int q = 0;

  Gain::Gain() {
    signal = add_input("signal");
    signal = add_output("signal");
    amplitude = add_input("amplitude");
  }

  void Gain::init() {
    set_input(amplitude, 0.5);
  }

  void Gain::update() {
    outputs[signal] = inputs[amplitude] * inputs[signal];      
  }    

}

