#ifndef YASE_COMPRESSOR_H
#define YASE_COMPRESSOR_H

#include "yase.hh"

namespace yase {

    class Compressor : public Module {

    public:

      Compressor();
      void init();
      void update();

    private:

      int signal;
      double y, yprev, gain, z;

    };

}

#endif
