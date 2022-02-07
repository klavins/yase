#ifndef YASE_CLOCK_H
#define YASE_CLOCK_H

#include "yase.hh"

namespace yase {

    class Clock : public Oscillator {

    public:

      Clock();
      void update();
    
    private:

      double prev;

    };

}

#endif
