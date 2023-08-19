// 
// YASE TappedDelay Module Header
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

#ifndef YASE_TAPPEDDELAY_H
#define YASE_TAPPEDDELAY_H

#include "yase.hh"

namespace yase {

    //! A linearly interpolated, tappable digital delay using a circular buffer. 
    
    //! This delay can have as many taps as you need. The taps can be specified
    //! by the amount of time to delay as a `double`, in seconds. When you add
    //! a new tap, you get a new output. The argument to the constructor is the
    //! total length of the delay and all tap positions must be less than that.
    //! For example,
    //! \code
    //! TappedDelay delay(1.0); // Store 1 second of signal
    //! auto [position,tap] = add_tap();
    //! delay.set_input(position, 0.5);
    //! \endcode
    //! Later, you can connect things to the delay as in
    //! \code
    //! container.connect(sound_source, delay)
    //!          .connect(delay, tap, sound_destination, "signal");
    //! \endcode    
    //! Since this is a linearly interpolated delay, the tap position can be 
    //! smoothly moved around. So you can also do something like
    //! \code
    //! container.connect(lfo, "signal", delay, position);
    //! \endcode
    //! to get neat effects. 
    //! \param[in] signal, position_0, position_1, ...
    //! \param[out] tap_0, tap_1, ...
    //! 

    class TappedDelay : public Module {

    public:

      TappedDelay(double duration);
      ~TappedDelay();
      void init();
      void update();
      tuple<string,string> add_tap();

    private:

      double duration;
      int duration_in_samples;
      int signal;
      double * buffer;
      int buffer_index;

      double tap_position(int i);
      void add_sample(double u);
      int fix(int n);
      double get_value(double position);

    };

}

#endif
