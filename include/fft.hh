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

    //! Compute an Fast Fourier Transform

    //! The Module can be used to compute a Fast Fourier Transform on its input.
    //! The resulting transform is available as a vector of fftw_complex numbers.
    //! See the documentation for the library fftw3. The data can also be printed
    //! to stdout using the ascii(), html(), or json() methods. 
    //! See the example in [examples/FFT](https://klavins.github.io/yase/examples/FFT)  for a demonstration. 
    //! \param[input] signal The input signal
    //! \param[output] ready 0 when the FFT is still collecting data, and 1 otherwise
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
