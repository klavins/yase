// 
// YASE Timer Module Header
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

#ifndef YASE_TIMER_H
#define YASE_TIMER_H

#include "yase.hh"

namespace yase {

    //! A timer that can be used to do something one or more times in the future.
    
    //! For example, the following prints "Time's up!" every second.
    //! \code{cpp}
    //! Timer timer;
    //! Container container;
    //! container.add(timer)
    //! timer.set(1, [&]() {
    //!     std::cout << "Time's up!";
    //!     timer.reset();
    //! });
    //! \endcode
    //!
    //! This module has no inputs or outputs
    class Timer : public Module {

    public:

      Timer();
      void set(float t, function<void()> f);
      void reset();
      void reset(float t);
      void init();
      void update();

    private:

      function<void()> handler;
      float duration;
      bool active;
      float timer;

    };

}

#endif
