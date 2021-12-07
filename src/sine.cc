#include <math.h>
#include "sine.hh"
#include "oscillator.hh"

namespace yase {

  void Sine::update() {
      Oscillator::update();
      outputs[signal] = sin(2 * M_PI * accumulator);
  }    

}

