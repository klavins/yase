// 
// YASE Oscillator Module Header
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

#ifndef YASE_OSCILLATOR_H
#define YASE_OSCILLATOR_H

#include "module.hh"

namespace yase {

    //! An abstract base class from which oscillators like Sine, Saw, and Triangle are derived

    //! This module provides methods and I/O that most oscillators need. It also updates
    //! a basic phasor in the protected variable `phase` which can be used by derived classes
    //! to build more complex signal outputs. 
    //! For a simple example see [include/square.hh](https://klavins.github.io/yase/include/square.hh)
    //! and [include/square.hh](https://klavins.github.io/yase/src/square.cc) which define a raw square wave oscillator.
    //! \param[input] frequency The frequency of the phasor
    //! \param[input] amplitude The amplitude of the oscillator (derived classes should scale their outputs by inputs[amplitude], it is not done automatically)
    //! \param[input] modulation A modulation input that modules that rate of the phasor
    //! \param[input] tuning This input has units in Cents. It tunes the input frequency of the oscillator.
    //! \param[input] harmonic The harmonic of the oscillator. The actual frequency will be frequency * 2^harmonic
    //! \param[output] signal The output of the oscillator

    class Oscillator : public Module {

    public:

      Oscillator();
      void init();
      virtual void update();

    protected:

      double phase;
      
      int frequency,       // local names for I/O indices
          signal,          // These need to be protected instead of private
          amplitude,       // so that derived classes can use them    
          modulation,
          tuning,
          harmonic;

    };

}

#endif
