#ifndef YASE_SUMMER_H
#define YASE_SUMMER_H

#include "yase.hh"

namespace yase {

    class Summer : public Module {

    public:

      Summer(int n);
      void init();
      void update();

    private:

      int size, signal;

    };

}

#endif
