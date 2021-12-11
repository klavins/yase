#ifndef YASE_VANDERPOL_H
#define YASE_VANDERPOL_H

#include "yase.hh"

namespace yase {

    class VanDerPol : public Oscillator {

    public:

      void init();
      void update();

    private:

      double x1, x2;

    };

}

#endif
