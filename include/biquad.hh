#ifndef YASE_BIOQUADLPF_H
#define YASE_BIOQUADLPF_H

#include "iir_filter.hh"
#include "yase.hh"

namespace yase {

    //! Standard Biquad filters adapated from
    //! https://www.w3.org/TR/audio-eq-cookbook/
    //! \param[in] signal
    //! \param[in] frequency
    //! \param[in] resonance
    //! \param[out] signal
    //! 
    class Biquad : public IIRFilter {

      typedef void (Biquad::*UpdateFunction)(); 

    public:

      Biquad();
      void init();
      void update();
      void on();
      void off();
      bool toggle();

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
      void bpf();

    };

}

#endif
