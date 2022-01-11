#ifndef YASE_DELAY_H
#define YASE_DELAY_H

#include "yase.hh"
#include <deque>

namespace yase {

    class Delay : public Module {

    public:

      Delay();
      void init();
      void update();

    private:

      deque<double> buffer;
      int signal, size;

    };

}

#endif
