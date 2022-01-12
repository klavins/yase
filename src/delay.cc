#include "delay.hh"
#include "yase.hh"

namespace yase {

  Delay::Delay(int duration) : duration(duration) {

    signal = add_input("signal");
    signal = add_output("signal");

  }

  //! Set the desired delay. 
  //! \param new_duration
  void Delay::set(int new_duration) {
    if ( new_duration > duration ) {
      // Nothing to do, just let the buffer grow
    } else if ( new_duration < duration ) {
      // Resize 
      while ( buffer.size() > new_duration ) {
        buffer.pop_back();
      }
    }
    duration = new_duration;
  }

  //! Check if the buffer is full
  //! \return True if the buffer size equals the duration
  bool Delay::is_full() {
    return buffer.size() >= duration;
  }

  void Delay::init() {

  }

  void Delay::update() {

    buffer.push_front(inputs[signal]);

    if ( buffer.size() > duration) {
        outputs[signal] = buffer.back();
        buffer.pop_back();
    }

    while ( buffer.size() > duration ) { // just in case
        buffer.pop_back();
    }       

  }    

}

