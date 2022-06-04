// 
// YASE Transform Module Header
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

#ifndef YASE_TRANSFORM_H
#define YASE_TRANSFORM_H

#include "yase.hh"

namespace yase {

    //! Transform an input signal with a stateless, user-defined function
    //! For example
    //! \code
    //! Transform invert( [] (double u) { return -u; });
    //! \endcode
    //! \param[in] signal
    //! \param[out] signal
    class Transform : public Module {

    public:

      Transform(function<double(const double u)>);
      void init();
      void update();

    private:

      function<double(const double u)> f;
      int signal;

    };

}

#endif
