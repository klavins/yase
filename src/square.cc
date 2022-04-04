#include "yase.hh"

namespace yase {

  void Square::update() {
    Oscillator::update();
    outputs[signal] = 0;
    int n = 1;
    while ( n * inputs[frequency] < 0.5 * SAMPLE_RATE ) {
      outputs[signal] += sin(n*2*M_PI*phase  + inputs[modulation])/n;
      n += 2;
    }
    outputs[signal] *= inputs[amplitude] * 4 / M_PI;
  }    

}

