#ifndef YASE_SIMPLE_H
#define YASE_SIMPLE_H

#include "yase.hh"

namespace yase {

    class Simple : public Container {

    public:

      Simple(json &config);
      void init();
      void update();
      void keydown(const Event &e);
      void keyup(const Event &e);      

    private:

      FaderManager controls;
      Saw osc;
      Envelope env;
      Biquad filter;

      int signal;

    };

}

#endif
