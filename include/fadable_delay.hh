#ifndef YASE_FADABLE_DELAY_H
#define YASE_FADABLE_DELAY_H

#include "delay.hh"
#include "container.hh"

namespace yase {

    //! A time modulatable delay. This module contains two delays running
    //! at the same time. When the "duration" input changes, one of the
    //! delay's duration is reset, and the output fades from the old
    //! delay to the new one. This approach is inspired by the `ddi` method
    //! (Double Delay with Interpolation) in Faust.
    //!
    //! Note that the duration is in samples. So to set up a control for a FadeDelay
    //! object that ranges from a milliscond to a second, you would do something like the
    //! following:
    //! ```
    //! controls.control(
    //!     fade_delay, 
    //!     "duration", 
    //!     0.001 * SAMPLE_RATE, 
    //!     SAMPLE_RATE, 
    //!     49);
    //! ```
    //! Where controls is a Controls object.
    //! 
    //! Inputs:
    //!   - "signal"
    //!   - "duration"
    //! 
    //! Outputs:
    //!   - "signal"
    class FadableDelay : public Container {

    public:

      typedef enum { NORMAL, RECORDING, FADING } STATE;
      const double FADE_TIME = 0.1;

      FadableDelay();
      void init();
      void update();

    private:

      int current, signal, duration; // indices
      int current_duration; 
      STATE state;
      Delay delays[2];
      double fade;
      int counter;

    };

}

#endif
 