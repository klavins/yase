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
