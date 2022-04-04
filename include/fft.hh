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
