#include <math.h>
#include "yase.hh"

namespace yase {

  Oscillator::Oscillator() {
      frequency = add_input("frequency");
      amplitude = add_input("amplitude");
      modulation = add_input("modulation");
      tuning = add_input("tuning");
      harmonic = add_input("harmonic");
      signal = add_output("signal");
      inputs[amplitude] = 1;
      inputs[frequency] = 440;
      inputs[modulation] = 0;
      inputs[tuning] = 0;
      inputs[harmonic] = 1;
  }

  void Oscillator::init() {
      accumulator = 0;
  }

  void Oscillator::update() {
      double h = pow(2, floor(inputs[harmonic]));
      accumulator += TS * (h*inputs[frequency] + CENT*floor(2*inputs[tuning])/2);
      if ( accumulator > 1 ) {
        accumulator -= 1;
      }     
  }

}

