#include "yase.hh"

namespace yase {

  Clock::Clock() {
    inputs[frequency] = 1;
    outputs[signal] = 5.0;
  }

  void Clock::update() {
      Oscillator::update(); 
      if ( phase < prev ) {
        outputs[signal] *= -5.0;
      }
      prev = phase;
  }    

}