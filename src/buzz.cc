#include <math.h>
#include "buzz.hh"

namespace yase {

  Buzz::Buzz() : Oscillator() {
    duty_factor = add_input("duty_factor");
    inputs[duty_factor] = 0.5;
  }

  void Buzz::update() {

    Oscillator::update();

    outputs[signal] = 0;
    double n = 1;
    while ( n < 40 ) {
      outputs[signal] += sin(n*2*M_PI*accumulator + inputs[modulation]);
      n += 1.0/8;
    }    

    // if ( accumulator < 0.2 ) {
    //   outputs[signal] = 1;
    // } else {
    //   outputs[signal] = -1;
    // }

    outputs[signal] = inputs[amplitude] * outputs[signal];

  }    

}