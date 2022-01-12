#ifndef YASE_DELAY_H
#define YASE_DELAY_H

#include "module.hh"
#include <deque>

using namespace std;

namespace yase {

    //! A pure digital delay using a doubled ended queue. This module is good for a fixed delay.
    //! If you want a modulatable delay, use FadeDelay, which has a "duration" input.
    //! 
     //! Inputs: 
     //!   - "signal"
     //! 
     //! Outputs: 
     //!   - "signal"
     //!     
    class Delay : public Module {

    public:

      Delay(int duration);
      void init();
      void update();

      void set(int new_duration);
      bool is_full();

    private:

      deque<double> buffer;
      int signal, duration;

    };

}

#endif
