#ifndef YASE_CLOCK_H
#define YASE_CLOCK_H

#include "yase.hh"

namespace yase {

    //! A raw (aliasing) square wave alternating between -1 and 1. 
    //! Can be used to dub sequencers or even external modular 
    //! systems if sent to an aux audio output. 
    //!
    //! \param[in] frequency
    //! \param[out] signal
    //! 
    class Clock : public Oscillator {

    public:

      Clock();
      void update();
    
    private:

      double prev;

    };

}

#endif
