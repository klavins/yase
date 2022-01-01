#ifndef YASE_RESONATOR_H
#define YASE_RESONATOR_H

#include "iir_filter.hh"

namespace yase {

    class Resonator : public IIRFilter {

    public:

      Resonator();
      void init();
      void update();

    private:

      int radius, theta;

    };

}

#endif
