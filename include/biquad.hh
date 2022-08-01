// 
// YASE Biquad Module Header
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

#ifndef YASE_BIOQUADLPF_H
#define YASE_BIOQUADLPF_H

#include "iir_filter.hh"
#include "yase.hh"

namespace yase {

    //! Standard Biquad filters adapated from https://www.w3.org/TR/audio-eq-cookbook/

    //! \param[in] signal
    //! \param[in] frequency
    //! \param[in] resonance
    //! \param[out] signal
    //! 
    class Biquad : public IIRFilter {

      typedef void (Biquad::*UpdateFunction)(); 

    public:

      Biquad();
      void init();
      void update();
      void on();
      void off();
      bool toggle();

      void set_type(std::string name);
      void recalculate();

    private:

      UpdateFunction update_fcn;

      double prev_freq,
             prev_res,
             cosw,
             alpha;
      int frequency, resonance;
      bool active, changed;

      void lpf();
      void hpf();
      void apf();
      void bpf();

    };

}

#endif
