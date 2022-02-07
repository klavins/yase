#include "yase.hh"

namespace yase {

  Clock::Clock() {
    outputs[signal] = 1.0;
  }

  void Clock::update() {
      Oscillator::update(); 
      if ( accumulator < prev ) {
        outputs[signal] *= -1.0;
      }
      prev = accumulator;
  }    

}