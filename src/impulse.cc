#include "impulse.hh"
#include "yase.hh"
#include <stdlib.h> 

namespace yase {

  Impulse::Impulse() : length(1) {
    signal = add_output("signal");
    update_fcn = &Impulse::square;
  }

  void Impulse::init() {}

  void Impulse::update() {
    CALL_MEMBER_FN(this, update_fcn);
  }

  void Impulse::set_type(std::string type) {
    if ( type == "random" ) {
      update_fcn = &Impulse::random;
    } else {
      update_fcn = &Impulse::square;
    }
  }

  void Impulse::square() {
    if ( n++ < length ) {
      outputs[signal] = 1;
    } else {
      outputs[signal] = 0;
    }
  }    

  void Impulse::random() {
    if ( n++ < length ) {
      outputs[signal] = 0.001 * (rand()%1000);
    } else {
      outputs[signal] = 0;
    }
  }    

  void Impulse::trigger() {
    n = 0;
  }

  void Impulse::set(int k) {
    length = k;
  }  

}

