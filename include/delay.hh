// 
// YASE Delay Module Header
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

#ifndef YASE_DELAY_H
#define YASE_DELAY_H

#include "module.hh"
#include <deque>

using namespace std;

namespace yase {

    //! A pure digital delay using a doubled ended queue. 
    
    //! This module is good for a fixed delay.
    //! If you want a modulatable delay, use FadeDelay, which has a "duration" input.
    //! 
    //! \param[in] signal
    //! \param[out] signal
    //!     
    class Delay : public Module {

    public:

      Delay(int duration);
      Delay();
      void init();
      void update();
      void clear();

      void set(int new_duration);
      bool is_full();

    private:

      deque<double> buffer;
      int signal, duration;

    };

}

#endif
