#include "oscillator.hh"
#include "module.hh"

namespace yase {

  Oscillator::Oscillator() {
      frequency = add_input("frequency");
      signal = add_output("signal");
  }

  void Oscillator::init() {
      n = 0;
  }

}

