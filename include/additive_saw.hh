// 
// YASE Saw Module Header
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

#ifndef YASE_ADDITIVE_SAW_H
#define YASE_ADDITIVE_SAW_H

#include <string.h>
#include "yase.hh"

namespace yase {

    //! A sawtooth wave oscillator

    //! This module produces a sawtooth wave output and has all the inputs that
    //! an Oscillator has. It  sums up sine waves according to the fourier expansion of a sawtooth wave. This is computationally expensive but very nice; or
    //! \param[input] frequency The frequency
    //! \param[input] amplitude The amplitude
    //! \param[input] modulation A modulation input that modules that rate of the square wave
    //! \param[input] tuning This input has units in Cents. It tunes the input frequency of the oscillator.
    //! \param[input] harmonic The harmonic of the oscillator. The actual frequency will be frequency * 2^harmonic
    //! \param[output] signal The output of the oscillator    

    class AdditiveSaw : public Oscillator {

    public:

      AdditiveSaw();
      void init();
      void update();

    };

}

#endif
