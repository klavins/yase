// 
// YASE Clock Module Header
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

#ifndef YASE_CLOCK_H
#define YASE_CLOCK_H

#include "yase.hh"

namespace yase {

    //! A raw (aliasing) square wave alternating between -1 and 1. 
    
    //! Can be used to clock sequencers or even external modular 
    //! systems if sent to an aux audio output. 
    //!
    //! \param[in] frequency
    //! \param[out] signal
    //! 
    class Clock : public Oscillator {

    public:

      Clock();
      void update();
    
    private:

      double prev;

    };

}

#endif
