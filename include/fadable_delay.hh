// 
// YASE Fadable Delay Module Header
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

#ifndef YASE_FADABLE_DELAY_H
#define YASE_FADABLE_DELAY_H

#include "delay.hh"
#include "container.hh"

namespace yase {

    //! A time modulatable delay.
    
    //! This module contains two delays running
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
    //!     fadable_delay, 
    //!     "duration", 
    //!     0.001 * SAMPLE_RATE, 
    //!     SAMPLE_RATE, 
    //!     49);
    //! ```
    //! Where controls is a Controls object and "49" refers to a midi input id.
    //! 
    //! \param[in] signal
    //! \param[in] duration
    //! \param[out] signal
    //!
    class FadableDelay : public Container {

    public:

      typedef enum { NORMAL, RECORDING, FADING } STATE;
      const double FADE_TIME = 0.1;

      FadableDelay();
      void init();
      void update();

    private:

      int current, signal, in_signal, duration; // indices
      int current_duration; 
      STATE state;
      Delay delays[2];
      double fade;
      int counter;

    };

}

#endif
 