#ifndef YASE_BIOQUADLPF_H
#define YASE_BIOQUADLPF_H

#include "iir_filter.hh"
#include "yase.hh"

namespace yase {

    class Biquad : public IIRFilter {

      // From https://www.w3.org/TR/audio-eq-cookbook/

    typedef void (Biquad::*UpdateFunction)(); 

    public:

      Biquad();
      void init();
      void update();
      void on();
      void off();
      void toggle();

      void set_type(std::string name);
      void recalculate();

    private:

      UpdateFunction update_fcn;

      double prev_freq,
             prev_res,
             cosw,
             alpha;
      int frequency, resonance;
      bool active, changed;

      void lpf();
      void hpf();
      void apf();

    };

}

#endif