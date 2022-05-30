#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "fader.hh"
#include "yase.hh"

namespace yase {

  Fader::Fader(double min, double max) : min_val(min), max_val(max), is_linear(true), tracking_gain(FADER_GAIN) {
    target = add_input("target");
    value = add_output("value");
    set_input(target,0); 
  }

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

  void Fader::randomize() {
    set_input(target, rand() % 127 );
  }

}
