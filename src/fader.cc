#include <iostream>
#include <math.h>
#include "fader.hh"
#include "yase.hh"

namespace yase {

  Fader::Fader(double min, double max) : min_val(min), max_val(max), inverted(false) {
    target = add_input("target");
    value = add_output("value");
    set_input(target,64); // Default position for fader or knob is straight up    
  }

  Fader::Fader() : min_val(0), max_val(1), inverted(false) {
    target = add_input("target");
    value = add_output("value");
    set_input(target,64); // Default position for fader or knob is straight up    
  }

  Fader::Fader(double min, double max, bool inverted) : min_val(min), max_val(max), inverted(inverted) {
    target = add_input("target");
    value = add_output("value");    
    if ( inverted && min == 0.0 ) {
      throw Exception("Cannot have an inverted input with a minimum value of zero.");
    }
  }

  void Fader::init() {
    outputs[value] = adjusted_target();
  }

  double Fader::adjusted_target() {
    double v;
    if ( inverted ) {
      v = 127 - inputs[target];
    } else {
      v = inputs[target];
    }
    return min_val + ( max_val-min_val ) * v / 127.0;
  }

  void Fader::update() {

    outputs[value] -= TS * FADER_GAIN * (outputs[value] - adjusted_target());

  }    

}
