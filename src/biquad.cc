// 
// YASE BiQuad Module Implementation
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
#include "iir_filter.hh"
#include "yase.hh"

namespace yase {

  Biquad::Biquad() : IIRFilter (3,3), active(true) {

    frequency = add_input("frequency");
    resonance = add_input("resonance");

    set_input(frequency, 1000);
    set_input(resonance, 10);    

    update_fcn = &Biquad::lpf;   

  }

  //! Set the type of the filter to be either low pass, high pass, all
  //! pass, or band pass.
  //! \param name Either "lpf", "hpf", "apf", or "bpf"
  void Biquad::set_type(std::string name) {

    if ( name == "lpf" )  {
      update_fcn = &Biquad::lpf;
    } else if ( name == "hpf" ) {
      update_fcn = &Biquad::hpf;      
    } else if ( name == "apf" ) {
      update_fcn = &Biquad::apf;
    } else if ( name == "bpf" ) {
      update_fcn = &Biquad::bpf;
    } else {
      update_fcn = &Biquad::lpf;
    }

    changed = true;

  } 

  //! Turn the filter on. 
  void Biquad::on() {
    active = true;
  }

  //! Turn the filter off. In it's off state, the input signal
  //! is routed directly to the output signal.
  void Biquad::off() {
    active = false;
  }

  //! Turn the filter on if it is off and off if it is on.
  bool Biquad::toggle() {
    active = !active;
    return active;
  }

  void Biquad::init() {
    std::cout << "Initializing biquad filter with " << inputs[frequency] << ", " << inputs[resonance] << "\n";
    prev_freq = -1; // force coefficient update
    prev_res = -1;  // in first call to update()
  }

  void Biquad::update() {

    CALL_MEMBER_FN(this, update_fcn);    

    if ( active ) {
      IIRFilter::update();
    } else {
      outputs[signal] = inputs[signal];
    }

  }

  //! Recalculate the coefficients of the filter. Generally this
  //! is done automatically when the frequency or resonance input
  //! changes.
  void Biquad::recalculate() {

    // std::cout << "Recalculating biquad filter parameters from " << inputs[frequency] << ", " << inputs[resonance] << "\n";
    // std::cout << "  Active: " << (active ? "yes" : "no") << "\n";

    double f0 = inputs[frequency] + inputs[offset],
            Q = inputs[resonance] > 0 ? inputs[resonance] : 1,                 
           w0 = 2 * M_PI * f0 * ts;

    std::cout << "  Params: " << cosw << ", " << alpha << " ==> ";  

    cosw = cos(w0);
    alpha = sin(w0) / ( 2 * Q );

    // std::cout << cosw << ", " << alpha << "\n";  

    prev_freq = inputs[frequency];
    prev_res = inputs[resonance];    
    changed = false;

  }

  void Biquad::lpf() {

    if ( prev_freq != inputs[frequency] || prev_res != inputs[resonance] || changed ) {

      recalculate(); 
      b[0] = (1-cosw) / 2,
      b[1] = 1 - cosw,
      b[2] = b[0],
      a[0] = 1 + alpha,
      a[1] = -2 * cosw,
      a[2] = 1 - alpha;      

    }

  }    

  void Biquad::hpf() {

    if ( prev_freq != inputs[frequency] || prev_res != inputs[resonance] || changed ) {

      recalculate();
      b[0] = (1+cosw) / 2,
      b[1] = -(1 + cosw),
      b[2] = (1+cosw) / 2,
      a[0] = 1 + alpha,
      a[1] = -2 * cosw,
      a[2] = 1 - alpha;     

    }

  }    

  void Biquad::bpf() {

    if ( prev_freq != inputs[frequency] || prev_res != inputs[resonance] || changed ) {

      recalculate();
      b[0] = inputs[resonance] * alpha,
      b[1] = 0,
      b[2] = -inputs[resonance] * alpha,
      a[0] = 1 + alpha,
      a[1] = -2 * cosw,
      a[2] = 1 - alpha;     

    }

  }     

  void Biquad::apf() {

    if ( prev_freq != inputs[frequency] || prev_res != inputs[resonance] || changed ) {

      recalculate();
      b[0] = 1 - alpha,
      b[1] = -2 * cosw,
      b[2] = 1 + alpha,
      a[0] = 1 + alpha,
      a[1] = -2 * cosw,
      a[2] = 1 - alpha;

    }    

  }

}