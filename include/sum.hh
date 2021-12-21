#ifndef YASE_SUMMER_H
#define YASE_SUMMER_H

#include "module.hh"

namespace yase {

    class Sum : public Module {

    public:

      Sum(int n);
      void init();
      void update();

    private:

      int size, signal;

    };

}

#endif
