#ifndef YASE_BIOQUADLPF_H
#define YASE_BIOQUADLPF_H

#include "filter.hh"
#include "yase.hh"

namespace yase {

    class BiquadLPF : public Filter {

      // From https://www.w3.org/TR/audio-eq-cookbook/

    public:

      BiquadLPF();
      void init();
      void update();

    private:

      double prev_freq, prev_res;
      int frequency, resonance;

    };

}

#endif
