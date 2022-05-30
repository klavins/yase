#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "fader.hh"
#include "yase.hh"

namespace yase {

  //! Construct a new fader. The default response is linear. And the default tracking gain is TRACKING_GAIN.
  //! \param min The minimum value of the fader
  //! \param max The maximum value of the fader
  Fader::Fader(double min, double max) : min_val(min), max_val(max), is_linear(true), tracking_gain(FADER_GAIN) {
    target = add_input("target");
    value = add_output("value");
    set_input(target,0); 
  }

  //! Construct a new fader with range [0,1]. The default response is linear. And the default tracking gain is TRACKING_GAIN.
  Fader::Fader() : min_val(0), max_val(1), is_linear(true) {
    target = add_input("target");
    value = add_output("value");
    set_input(target,0); 
  }

  void Fader::init() {
    outputs[value] = adjusted_target();
  }

  double Fader::adjusted_target() {
    double v = inputs[target];
    if ( is_linear ) {
      return min_val + ( max_val-min_val ) * v / 127.0;
    } else {
      double temp = pow(base,v/127.0);
      return (max_val-min_val)*(temp-1)/(base-1)+min_val;      
    }
  }

  void Fader::update() {
    outputs[value] -= ts * tracking_gain * (outputs[value] - adjusted_target());
  }    

  //! Set the target of the fader to a random value from within its min and max values. 
  void Fader::randomize() {
    set_input(target, rand() % 127 );
  }

}
