#include <iostream>
#include "yase.hh"

namespace yase {

  void DirtyTriangle::update() {
      Oscillator::update();
      double temp;
      if ( accumulator < 0.5 ) {
        temp = accumulator;
      } else {
        temp = 1 - accumulator;
      }
      outputs[signal] = inputs[amplitude] * ( 4 * temp - 1 );
  }    

}

