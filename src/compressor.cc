#include <math.h>
#include "compressor.hh"
#include "yase.hh"

namespace yase {

  Compressor::Compressor() {

    signal = add_input("signal");
    signal = add_output("signal");

  }

  void Compressor::init() {

  }

  int n = 0;

  void Compressor::update() {

    double alpha = 0.5,
           T = 0.5,
           W = 10,
           R = 10;

    // udpate y
    y = alpha * yprev + (1-alpha) * abs(inputs[signal]);
    yprev = y;

    z = y;

    // find the gain
    if ( z < T ) {
        gain = z;
    } else { 
        gain = T+(z-T)/R;
    }

    // apply new gain
    outputs[signal] = gain * inputs[signal];

    // if ( n++ < 0.25 * SAMPLE_RATE ) {
    //   std::cout << inputs[signal] << ", " << y << ", " << z << ", " << gain << "\n";
    //   n = 0;
    // }

  }    

}

