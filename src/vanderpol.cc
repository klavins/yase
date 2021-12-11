#include <math.h>
#include "yase.hh"

namespace yase {

  int n = 0;

  void VanDerPol::init() {
    Oscillator::init();
    x1 = 1.0;
    x2 = 0.0;
  }

  void VanDerPol::update() {
    Oscillator::update();
    x1 += 10*inputs[frequency]*TS * x2;
    x2 += 10*inputs[frequency]*TS * ( 
      + 0.2 * M_PI * (1-x1*x1)*x2 
      - x1 
      + 1.2 * sin(2*M_PI*accumulator) );
    outputs[signal] = inputs[amplitude] * x2 / 4;
  }    

}

