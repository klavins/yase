#include <iostream>
#include <math.h>
#include "sine.hh"
#include "oscillator.hh"

int p = 0;

namespace yase {

  void Sine::update() {
      Oscillator::update();
      outputs[signal] = inputs[amplitude] 
                      * sin(2 * M_PI * phase  + inputs[modulation]);
  }    

}