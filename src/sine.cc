#include <math.h>
#include "sine.hh"
#include "oscillator.hh"

namespace yase {

  void Sine::update() {
      outputs[signal] = sin(1 * 2 * M_PI * inputs[frequency] * n * TS);
      n++;
  }    

  void Sine::change_frequency(double f) {
    n = (inputs[frequency] / f) * n;
    inputs[frequency] = f;
  }

}

