#ifndef YASE_ECHO_H
#define YASE_ECHO_H

#include "yase.hh"

namespace yase {

    //! An echo module incorporating a fadable delay in a feedback configuration
    //! \param[in] duration
    //! \param[in] gain
    //! \param[in] signal
    //! \param[out] signal
    class Echo : public Container {

    public:

      Echo();
      void init();
      void update();

    private:

      Sum sum;
      FadableDelay delay;
      Gain gain; 

      int signal,         // Indices of inputs and outputs
          amplitude, 
          duration;

    };

}

#endif
