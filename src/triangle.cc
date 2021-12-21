#include "yase.hh"

namespace yase {

  Triangle::Triangle() : Oscillator() {
    update_fcn = &Triangle::additive;
  }

  void Triangle::init() {
    Oscillator::init();
  }

  void Triangle::update() {
    Oscillator::update();
    CALL_MEMBER_FN(this, update_fcn);
    outputs[signal] *= inputs[amplitude];
  }    

  void Triangle::set_type(std::string name) {

    if ( name == "raw" )  {
      update_fcn = &Triangle::raw;
    } else if ( name == "additive" ) {
      update_fcn = &Triangle::additive;
    } else if ( name == "ptr1" ) {
      update_fcn = &Triangle::ptr1;
    } else {
      update_fcn = &Triangle::ptr1;
    }

  }    

  void Triangle::raw() {

  }    

  void Triangle::ptr1() {

  }    

  void Triangle::additive() {
   
    outputs[signal] = 0;
    int n = 1;
    while ( n * inputs[frequency] < SAMPLE_RATE / 2 ) {
      outputs[signal] += sin(n*2*M_PI*accumulator + inputs[modulation])/(n*n);
      n += 2;
    }
    outputs[signal] *= 8/(M_PI*M_PI);

  }    

}

