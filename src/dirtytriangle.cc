#include "yase.hh"

namespace yase {

  void DirtyTriangle::update() {
      Oscillator::update();
      outputs[signal] = 2 * accumulator - 1;
  }    

}

