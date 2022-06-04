// 
// YASE Cycle Module Header
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

#ifndef YASE_CYCLE_H
#define YASE_CYCLE_H

#include "yase.hh"

namespace yase {

    //! Cycle through a list of pitches, running a callback for each one with the specified duration.
    class Cycle : public Module {

    public:

      Cycle();
      void set(vector<double> pitch_list, function<void(double)> f, double dt);
      void init();
      void update();

    private:

      function<void(double)> callback;
      double duration, t;
      vector<double> pitches;
      int n;

    };

}

#endif
