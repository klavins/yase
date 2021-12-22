#include "envelope.hh"
#include "yase.hh"

#define OUTPUT outputs[signal]
#define INPUT inputs[signal]
#define A inputs[a]
#define D inputs[d]
#define S inputs[s]
#define R inputs[r]
#define VELOCITY inputs[velocity]

namespace yase {

  Envelope::Envelope() {

    signal = add_input("signal");
    signal = add_output("signal"); // should both be 0

    a = add_input("attack");
    d = add_input("decay");
    s = add_input("sustain");
    r = add_input("release");

    velocity = add_input("velocity");

    // Defaults
    set_input(a, 100); // Rate of attack in % per sample
    set_input(d, 1);
    set_input(s, 1);
    set_input(r, 30);
    set_input(velocity, 1);    

  }

  void Envelope::init() {
    update_fcn = &Envelope::off;
  }

  void Envelope::trigger() {
    update_fcn = &Envelope::attack;
  }  

  void Envelope::release() {
    update_fcn = &Envelope::_release;
  }  

  void Envelope::update() {
    CALL_MEMBER_FN(this, update_fcn);
    OUTPUT = amplitude * INPUT;
  }     

  void Envelope::off() {
    // Do nothing
  }

  void Envelope::attack() {
    amplitude += TS * A;
    if ( amplitude >= VELOCITY ) {
      update_fcn = &Envelope::decay;
    }
  }

  void Envelope::decay() {
    amplitude -= TS * D * ( amplitude - S * VELOCITY );
    if ( amplitude <= S * VELOCITY + ENV_EPS ) {
        update_fcn = &Envelope::sustain;
    }
  }

  void Envelope::sustain() {
    // do nothing
  }
  
  void Envelope::_release() {
    amplitude -= TS * R * amplitude;
    if ( amplitude <= ENV_EPS ) {
      amplitude = 0.0;
      update_fcn = &Envelope::off;
    }
  }   

}