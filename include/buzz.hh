#ifndef YASE_BUZZ_H
#define YASE_BUZZ_H

#include "yase.hh"

namespace yase {

    class Buzz : public Oscillator {

    public:

      Buzz();
      void update();

    private:

      int duty_factor;

    };

}

#endif
