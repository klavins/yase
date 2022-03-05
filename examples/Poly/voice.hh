#ifndef YASE_VOICE_H
#define YASE_VOICE_H

#include "yase.hh"

namespace yase {

    class Voice : public Container {

    public:

      Voice(json &config);
      void init();
      void update();
      void keydown(const Event &e);
      void keyup(const Event &e); 

    private:

      Controls controls;
      Wavetable osc;
      Envelope env;
      Biquad filter;

      int signal;

    };

}

#endif
