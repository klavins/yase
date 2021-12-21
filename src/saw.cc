#include <math.h>
#include "yase.hh"

namespace yase {

  Saw::Saw() : Oscillator() {
    update_fcn = &Saw::additive;
  }  

  void Saw::init() {
    Oscillator::init();
  }

  void Saw::update() {
    Oscillator::update();
    CALL_MEMBER_FN(this, update_fcn);
    outputs[signal] *= inputs[amplitude];
  }    

  void Saw::set_type(std::string name) {

    if ( name == "raw" )  {
      update_fcn = &Saw::raw;
    } else if ( name == "additive" ) {
      update_fcn = &Saw::additive;
    } else if ( name == "ptr1" ) {
      update_fcn = &Saw::ptr1;
    } else {
      update_fcn = &Saw::ptr1;
    }

  }    

  void Saw::raw() {
      outputs[signal] = inputs[amplitude] * ( 2 * accumulator - 1 );
  }    

  void Saw::ptr1() {
    double h = 1,
           P0 = SAMPLE_RATE/inputs[frequency],  // 44100 / 440 = 100
           T0 = 1/P0,                           // 0.1
           cdc = T0,                            // 0.1
           DC = 1 + cdc,                        // 1.1
           a1 = 2 - 2*h*P0,                     // 2 - 2*100 = -198
           a0 = 2*h - DC;                       // 1 
    if ( accumulator >= T0 ) {
        outputs[signal] = 2*accumulator - DC;
    } else {
        outputs[signal] = a1*accumulator + a0;
    }
  }    

  void Saw::additive() {
    outputs[signal] = 0;
    int n = 1;
    while ( n * inputs[frequency] < SAMPLE_RATE / 2 ) {
      outputs[signal] += sin(n*2*M_PI*accumulator)/n;
      n++;
    }
  }    

}
