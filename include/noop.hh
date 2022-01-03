#ifndef YASE_NOOP_H
#define YASE_NOOP_H

#include "yase.hh"

namespace yase {

    class Noop : public Module {

    public:

      Noop();
      void init();
      void update();

    private:

    };

}

#endif
