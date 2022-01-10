#ifndef YASE_NOOP_H
#define YASE_NOOP_H

#include "yase.hh"

namespace yase {


    //! A module that does almost nothing. It is used by the 'inject'
    //! method of the Mono synth to emit events that the listeners
    //! will respond to. Otherwise generally useless. 
    class Noop : public Module {

    public:

      Noop();
      void init();
      void update();

    private:

    };

}

#endif
