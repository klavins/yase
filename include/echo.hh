#ifndef YASE_ECHO_H
#define YASE_ECHO_H

#include "yase.hh"

namespace yase {

    class Echo : public Container {

    public:

      Echo();
      void init();
      void update();

    private:

      Sum sum;
      FadableDelay delay;
      Gain gain; 

      int signal,         // Indices of inputs and outputs
          amplitude, 
          duration;

    };

}

#endif
