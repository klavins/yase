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
      
      int frequency,       // local names for I/O indices
          signal,          // These need to be protected instead of private
          amplitude,       // so that derived classes can use them    
          modulation;

    };

}

#endif
