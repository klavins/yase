#ifndef YASE_OSCILLATOR_H
#define YASE_OSCILLATOR_H

#include "module.hh"

namespace yase {

    class Oscillator : public Module {

    public:

      Oscillator();
      void init();
      virtual void update();

    protected:

      double accumulator;
      int frequency, signal; // local names for I/O indices
                             // These need to be protected instead of private
                             // so that derived classes can use them    

    };

}

#endif
