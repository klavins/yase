// 
// YASE AntiAlias Module Header
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

#ifndef YASE_ANTIALIAS_H
#define YASE_ANTIALIAS_H

#include "biquad.hh"
#include "yase.hh"

namespace yase {

    //! An anti-aliasing filter. 

    //! This module wraps around other modules duplicating their inputs and providing
    //! a single output named "signal". The module works by running at twice the normal
    //! sample rate and using a biquad filter to filter out frequencies lower than
    //! half the sample rate. For example, to use it with a raw sawtooth oscillator,
    //! which is notorious for producing aliased frequencies, you would do
    //! \code
    //! Saw raw_saw("raw");
    //! AntiAlias saw(raw_saw);
    //! Container container;
    //! Audio audio;
    //! container.connect(saw, "signal", audio, "left");
    //! \endcode
    //! Note that you should add the AntiAlias Module to whatever container you 
    //! you building and not the underlying module. If you add both, you're 
    //! underlying module will be updated twice per timestep. 
    //!
    //! \param[in] * all parameters of underlying module
    //! \param[out] signal the anti-aliased output of the underlying module
    //!

    class AntiAlias : public Module {

    public:

      AntiAlias(Module &m);
      void init();
      void update();
      void set(double frequency, double resonance);

    private:

      int signal, 
          module_signal_out, 
          lowpass_signal_in,
          lowpass_signal_out;
      Module * module;
      Biquad lowpass;

    };

}

#endif
