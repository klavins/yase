// 
// YASE Triangle Wave Header
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

#ifndef YASE_TRIANGLE_H
#define YASE_TRIANGLE_H

#include <string.h>
#include "yase.hh"

namespace yase {

    //! A triangle wave oscillator

    //! This module produces a triangle  wave output and has all the inputs that
    //! an Oscillator has. The update method is either 
    //! - "raw", which should be wrapped with an AntiAlias Module;
    //! - "additive", which sums up sine waves according to the fourier expansion of a triangle wave. This is computationally expensive but very nice; or
    //! - "ptr1", which approximated a square wave with a rounded shape using a standard algorithm from the literature [TODO: find the paper for this and cite it]
    //! \param[input] frequency The frequency
    //! \param[input] amplitude The amplitude
    //! \param[input] modulation A modulation input that modules that rate of the square wave
    //! \param[input] tuning This input has units in Cents. It tunes the input frequency of the oscillator.
    //! \param[input] harmonic The harmonic of the oscillator. The actual frequency will be frequency * 2^harmonic
    //! \param[output] signal The output of the oscillator      

    class Triangle : public Oscillator {

    typedef  void (Triangle::*UpdateFunction)(); 

    public:
      Triangle();
      void init();
      void update();
      void set_type(std::string name);

    private:

      UpdateFunction update_fcn;

      void raw();
      void ptr1();
      void additive();

    };

}

#endif
