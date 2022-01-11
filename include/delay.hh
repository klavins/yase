#ifndef YASE_DELAY_H
#define YASE_DELAY_H

#include "yase.hh"
#include <deque>

namespace yase {

    //! A pure digital delay using a doubled ended queue. 
     //! Inputs: 
     //!   - "signal"
     //!   - "duration"
     //! 
     //! Outputs: 
     //!   - "signal"
     //!     
    class Delay : public Module {

    public:

      Delay();
      void init();
      void update();

    private:

      deque<double> buffer;
      int signal, duration;

    };

}

#endif
