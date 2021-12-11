#include <iostream>
#include "yase.hh"

namespace yase {


  void DirtySaw::update() {
      Oscillator::update();
      outputs[signal] = inputs[amplitude] * ( 2 * accumulator - 1 );
  }    

}

