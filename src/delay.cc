#include "delay.hh"
#include "yase.hh"

namespace yase {

  Delay::Delay() {

    signal = add_input("signal");
    size = add_input("size");
    signal = add_output("signal");

  }

  void Delay::init() {

  }

  void Delay::update() {

    buffer.push_front(inputs[signal]);

    if ( buffer.size() >= inputs[size] ) {
        outputs[signal] = buffer.back();
        buffer.pop_back();
    }

    while ( buffer.size() >= inputs[size] ) {
        buffer.pop_back();
    }    

  }    

}

