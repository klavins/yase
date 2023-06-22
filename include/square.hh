// 
// YASE Square Module Header
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

#ifndef YASE_SQUARE_H
#define YASE_SQUARE_H

#include "yase.hh"

namespace yase {

    //! A raw square wave oscillator

    //! This module produces a raw square wave output and has all the inputs that
    //! an Oscillator has. This Module is usually wrapped with an AntiAlias Module.
    //! \param[input] frequency The frequency
    //! \param[input] amplitude The amplitude
    //! \param[input] modulation A modulation input that modules that rate of the square wave
    //! \param[input] tuning This input has units in Cents. It tunes the input frequency of the oscillator.
    //! \param[input] harmonic The harmonic of the oscillator. The actual frequency will be frequency * 2^harmonic
    //! \param[output] signal The output of the oscillator    

    class Square : public Oscillator {

    public:

      void update();

    };

}

#endif
