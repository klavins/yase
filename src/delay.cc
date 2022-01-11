#include "delay.hh"
#include "yase.hh"

namespace yase {

  Delay::Delay() {

    signal = add_input("signal");
    duration = add_input("duration");
    signal = add_output("signal");

  }

  void Delay::init() {

  }

  void Delay::update() {

    buffer.push_front(inputs[signal]);

    if ( buffer.size() >= inputs[duration] ) {
        outputs[signal] = buffer.back();
        buffer.pop_back();
    }

    while ( buffer.size() >= inputs[duration] ) {
        buffer.pop_back();
    }    

  }    

}

