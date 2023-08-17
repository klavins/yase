// 
// YASE Impulse Module Header
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

#ifndef YASE_IMPULSE_H
#define YASE_IMPULSE_H

#include "yase.hh"

namespace yase {

    //! An impulse. 

    //! Outputs a non-zero signal for k time steps when triggered. The default value for
    //! k is 1. The output is either
    //! square or random depending on whether set_type("square") or set_type("random")
    //! has been called. The default is square. 
    //! \param[out] signal The output.

    class Impulse : public Triggerable {

    typedef void (Impulse::*UpdateFunction)(); 

    public:

      Impulse();

      void trigger();
      void release() {}

      void set(int k);
      void init();
      void update();
      void set_type(std::string);

    private:

      int length, n, signal;
      UpdateFunction update_fcn;   
      void square();
      void random();

    };

}

#endif
