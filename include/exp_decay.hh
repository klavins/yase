// 
// YASE ExpDecay Module Header
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

#ifndef YASE_EXPDECAY_H
#define YASE_EXPDECAY_H

#include "yase.hh"

namespace yase {

    //! An exponentially decaying envelope.
    
    //! This module produces an output named "signal" that exponentially decays 
    //! from the input "from" to the input "to" 
    //! according to the equations
    //! \code
    //! x = exp(-kt)
    //! y = u_from * x + u_to * ( 1 - x )
    //! \endcode
    //! Calling the method trigger() resets the envelope. 
    //! \param[in] rate The decay rate
    //! \param[in] from The starting value
    //! \param[in] to The ending value (in the limit as t gies to infinity)
    //! \param[out] signal The output signal
    class ExpDecay : public Module {

    public:

      ExpDecay();
      void trigger();
      void init();
      void update();

    private:

      int signal, rate, from, to;
      double value;

    };

}

#endif
