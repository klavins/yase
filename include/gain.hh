#ifndef YASE_GAIN_H
#define YASE_GAIN_H

#include "module.hh"

namespace yase {

     //! Amplifies (or attenuates) its input signal to get the output signal. This module
     //! Is often used to control the overall volume, and positioned as the last stage 
     //! before and Audio module. A MIDI fader would be hooked up to the "amplitude" input.
     //! 
     //! \param[in] signal
     //! \param[in] amplitude
     //! \param[out] signal
     //! 
    class Gain : public Module {

    public:

      Gain();
      void init();
      void update();

    private:

      int signal,
          amplitude;

    };

}

#endif
