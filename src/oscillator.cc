#include <math.h>
#include "yase.hh"

namespace yase {

  Oscillator::Oscillator() {
      frequency = add_input("frequency");
      amplitude = add_input("amplitude");
      modulation = add_input("modulation");
      tuning = add_input("tuning");
      signal = add_output("signal");
      inputs[amplitude] = 1;
      inputs[frequency] = 440;
      inputs[modulation] = 0;
      inputs[tuning] = 0;
  }

  void Oscillator::init() {
      accumulator = 0;
  }

  void Oscillator::update() {
      accumulator += TS * (inputs[frequency] + CENT*floor(2*inputs[tuning])/2);
      if ( accumulator > 1 ) {
        accumulator -= 1;
      }     
  }

}

