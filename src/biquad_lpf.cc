#include <math.h>
#include "filter.hh"
#include "yase.hh"

namespace yase {

  BiquadLPF::BiquadLPF() : Filter (3,3), active(true) {

    frequency = add_input("frequency");
    resonance = add_input("resonance");

    set_input(frequency, 1000);
    set_input(resonance, 10);   

  }

  void BiquadLPF::on() {
    active = true;
  }

  void BiquadLPF::off() {
    active = false;
  }

  void BiquadLPF::toggle() {
    active = !active;
  }

  void BiquadLPF::init() {
    prev_freq = -1; // force coefficient update
    prev_res = -1;  // in first call to update()
  }

  void BiquadLPF::update() {

    if ( prev_freq != inputs[frequency] || prev_res != inputs[resonance]) {
      double f0 = inputs[frequency] + inputs[offset],
             Q = inputs[resonance],
             w0 = 2 * M_PI * f0 / SAMPLE_RATE,
             cosw = cos(w0),
             alpha = sin(w0) / ( 2 * Q ),
             b0 = (1-cosw) / 2,
             b1 = 1 - cosw,
             b2 = b0,
             a0 = 1 + alpha,
             a1 = -2 * cosw,
             a2 = 1 - alpha;      

      set_coefficients({a0,a1,a2},{b0,b1,b2});

      prev_freq = inputs[frequency];
      prev_res = inputs[resonance];

    }

    if ( active ) {
      Filter::update();
    } else {
      outputs[signal] = inputs[signal];
    }

  }    

}

