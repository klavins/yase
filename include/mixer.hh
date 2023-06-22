// 
// YASE Mixer Module Header
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

#ifndef YASE_MIXER_H
#define YASE_MIXER_H

#include <vector>
#include "gain.hh"
#include "sum.hh"

namespace yase {

    //! A mixer

    //! Take n input signals and output the wieghted sum of the signals
    //! using the "gain_*" inputs. 
    //! \param[in] signal_i The ith signal
    //! \param[in] gain_i The ith gain
    //! \param[out] signal The output

    class Mixer : public Module {

    public:

      Mixer(int n);
      void init();
      void update();

      inline int amplitude_index(int i) { return n + i; }
      void set_amplitude_input(int i, double value); 
      double get_amplitude_input(int i);
      void extend();
      inline int size() { return n; }

    private:

      int n, 
          signal, 
          output_gain;

    };

}

#endif
