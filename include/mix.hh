// 
// YASE Mix Module Header
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

#ifndef YASE_MIX_H
#define YASE_MIX_H

#include "yase.hh" 

namespace yase {

    class Mix : public Container {

    public:

      //! A stereo mixer. 
      //! \param[in] connections -- a vector of tuples of the form
      //! { module, "signal_name", 1.23, 3.45 } where module is the module to 
      //! add, "signal_name" is the name of the signal coming out of the module,
      //! and the two numbers are the amplitudes for the left and right channels. 
      Mix(std::vector<std::tuple<Module&, string, double, double>> connections);

      void init();
      void update();

    private:

      Mixer mix_left, mix_right;

    };

}

#endif