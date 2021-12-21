#ifndef YASE_GAIN_H
#define YASE_GAIN_H

#include "module.hh"

namespace yase {

    class Gain : public Module {

    public:

      Gain();
      void init();
      void update();

    private:

      int signal,
          amplitude;

    };

}

#endif
