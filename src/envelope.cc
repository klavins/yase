#include <math.h>
#include "envelope.hh"
#include "yase.hh"

#define OUTPUT outputs[signal]
#define INPUT inputs[signal]
#define A ((pow(10.0,4*inputs[a]) - 1)/999.9)
#define D ((pow(10.0,4*inputs[d]) - 1)/999.9)
#define S inputs[s]
#define R ((pow(10.0,4*inputs[r]) - 1)/999.9)
#define VELOCITY inputs[velocity]

namespace yase {

  Envelope::Envelope() {

    // 
    // Linear attack and release
    // Exponential decay
    //

    signal = add_input("signal");
    signal = add_output("signal"); // should both be 0

    a = add_input("attack");
    d = add_input("decay");
    s = add_input("sustain");
    r = add_input("release");

    velocity = add_input("velocity");

    // Defaults
    set_input(a, 0.005);     // duration of attack
    set_input(d, 0.005);     // time to 90% decayed
    set_input(s, 1);         // percentage of max
    set_input(r, 0.005);     // duration of release

    set_input(velocity, 1);  // sometimes envelopes have no inputs so
    set_input(signal,1);     // we need defaults or we'll get no signal

    amplitude = 0;

    update_fcn = &Envelope::off;

  }

  void Envelope::init() {}

  void Envelope::trigger() {
    update_fcn = &Envelope::attack;
  }  

  void Envelope::release() {
    update_fcn = &Envelope::_release;
  }  

  void Envelope::update() {
    CALL_MEMBER_FN(this, update_fcn);
    OUTPUT = VELOCITY * amplitude * INPUT;
  }     

  void Envelope::off() {
    // Do nothing
  }

  void Envelope::attack() {
    amplitude += ts * (1/A);
    if ( amplitude >= 1 ) {
      amplitude = 1;
      update_fcn = &Envelope::decay;
    }
  }

  void Envelope::decay() {
    amplitude -= ts * (LN01/D) * ( amplitude - S * VELOCITY );
    if ( amplitude <= S + ENV_EPS ) {
        update_fcn = &Envelope::sustain;
    }
  }

  void Envelope::sustain() {
    // do nothing
  }
  
  void Envelope::_release() {
    amplitude -= ts * (LN01/R) * amplitude;
    if ( amplitude <= ENV_EPS ) {
      amplitude = 0.0;
      update_fcn = &Envelope::off;
     }
  }   

}