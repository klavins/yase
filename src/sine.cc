#include <math.h>
#include "sine.hh"
#include "oscillator.hh"

namespace yase {

  void Sine::update() {
      outputs[signal] = sin(1 * 2 * M_PI * inputs[frequency] * n * TS) 
                      + sin(2 * 2 * M_PI * inputs[frequency] * n * TS) / 2
                      + sin(3 * 2 * M_PI * inputs[frequency] * n * TS) / 3
                      + sin(4 * 2 * M_PI * inputs[frequency] * n * TS) / 4;
      n++;
  }    

  void Sine::change_frequency(double f) {
    n = (inputs[frequency] / f) * n;
    inputs[frequency] = f;
  }

}

