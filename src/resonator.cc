#include <math.h>
#include "resonator.hh"
#include "yase.hh"

namespace yase {

  Resonator::Resonator() : IIRFilter (3,3) {

    theta = add_input("theta");
    radius = add_input("radius");
    inputs[theta] = 1;
    inputs[radius] = 0.5;

  }

  void Resonator::init() {

  }

  void Resonator::update() {

    b[0] = 1;
    b[1] = 0;
    b[2] = -inputs[radius];

    a[0] = 1;
    a[1] = -2 * inputs[radius] * cos(inputs[theta]);
    a[2] = inputs[radius] * inputs[radius];

    IIRFilter::update();

  } 

}

