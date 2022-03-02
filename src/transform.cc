#include "transform.hh"
#include "yase.hh"

namespace yase {

  Transform::Transform(function<double(const double u)> f) : f(f) {
    signal = add_input("signal");
    signal = add_output("signal");
  }

  void Transform::init() {

  }

  void Transform::update() {
    outputs[signal] = f(inputs[signal]);
  }    

}

