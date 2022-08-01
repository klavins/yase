// 
// YASE FFT Module Header
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

#ifndef YASE_FFT_H
#define YASE_FFT_H

#include "yase.hh"
#include <fftw3.h>

namespace yase {

    class FFT : public Module {

    public:

      FFT(int size);
      ~FFT();
      void init();
      void update();
      void ascii();
      void html();
      std::string json();

      fftw_complex * get();

    private:


      double * in;
      fftw_complex *out;
      fftw_plan plan;
      int size, n;
      int signal, ready;

    };

}

#endif
