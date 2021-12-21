#include "yase.hh"

namespace yase {

  Oscillator::Oscillator() {
      frequency = add_input("frequency");
      amplitude = add_input("amplitude");
      modulation = add_input("modulation");
      signal = add_output("signal");
      inputs[amplitude] = 1;
      inputs[frequency] = 440;
      inputs[modulation] = 0;
  }

  void Oscillator::init() {
      accumulator = 0;
  }

  void Oscillator::update() {
      accumulator += TS * inputs[frequency];
      if ( accumulator > 1 ) {
        accumulator -= 1;
      }     
  }

}

