// 
// YASE Envelope Module Implementation
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

#include <math.h>
#include "envelope.hh"
#include "yase.hh"

#define OUTPUT outputs[out_signal]
#define INPUT inputs[in_signal]
#define A inputs[a]
#define D inputs[d]
#define S inputs[s]
#define R inputs[r]
#define VELOCITY inputs[velocity]

namespace yase {

  Envelope::Envelope() : Triggerable() {

    // 
    // Linear attack and release
    // Exponential decay
    //

    in_signal = add_input("signal");
    out_signal = add_output("signal"); // should both be 0

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
    set_input(in_signal,1);     // we need defaults or we'll get no signal
    amplitude = 0;

    update_fcn = &Envelope::off;

  }

  //! The equivalent of doing
  //!
  //! \code
  //!  set_input("attack", aa);
  //!  set_input("decay",  dd);
  //!  set_input("sustain", ss);
  //!  set_input("release", rr);  
  //! \endcode
  //!
  //! \param aa Attack duration
  //! \param dd Time to 90% decayed
  //! \param ss Percentage of maximum for the sustain
  //! \param rr To to 90% released
  void Envelope::set_adsr(double aa, double dd, double ss, double rr) {
    set_input("attack", aa);
    set_input("decay",  dd);
    set_input("sustain", ss);
    set_input("release", rr);    
  }

  void Envelope::init() {
      Triggerable::init();
  }

  void Envelope::trigger() {
    update_fcn = &Envelope::attack;
  }  

  void Envelope::release() {
    update_fcn = &Envelope::_release;
  }  

  void Envelope::update() {

    Triggerable::update();

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