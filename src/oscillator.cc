#include "yase.hh"

namespace yase {

  Oscillator::Oscillator() {
      frequency = add_input("frequency");
      signal = add_output("signal");
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

