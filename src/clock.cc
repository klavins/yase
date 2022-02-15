#include "yase.hh"

namespace yase {

  Clock::Clock() {
    inputs[frequency] = 1;
    outputs[signal] = 1.0;
  }

  void Clock::update() {
      Oscillator::update(); 
      if ( phase < prev ) {
        outputs[signal] *= -1.0;
      }
      prev = phase;
  }    

}