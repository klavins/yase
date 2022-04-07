#ifndef YASE_ANTIALIAS_H
#define YASE_ANTIALIAS_H

#include "biquad.hh"
#include "yase.hh"

namespace yase {

    class AntiAlias : public Module {

    public:

      AntiAlias(Module &m);
      void init();
      void update();
      void set(double frequency, double resonance);

    private:

      int signal, 
          module_signal_out, 
          lowpass_signal_in,
          lowpass_signal_out;
      Module * module;
      Biquad lowpass;

    };

}

#endif
