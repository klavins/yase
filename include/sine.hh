#ifndef YASE_SINE_H
#define YASE_SINE_H

#include "yase.hh"

namespace yase {

    class Sine : public Oscillator {

    public:

      void update();
      void change_frequency(double f);

    };

}

#endif
