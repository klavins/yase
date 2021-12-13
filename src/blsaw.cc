#include "yase.hh"

namespace yase {

  // http://research.spa.aalto.fi/publications/papers/spl-ptr/

  void BLSaw::update() {
    Oscillator::update();
    double h = 1,
           P0 = SAMPLE_RATE/inputs[frequency],  // 44100 / 440 = 100
           T0 = 1/P0,                           // 0.1
           cdc = T0,                            // 0.1
           DC = 1 + cdc,                        // 1.1
           a1 = 2 - 2*h*P0,                     // 2 - 2*100 = -198
           a0 = 2*h - DC;                       // 1 
    if ( accumulator >= T0 ) {
        outputs[signal] = 2*accumulator - DC;
    } else {
        outputs[signal] = a1*accumulator + a0;
    }
  }    

}
