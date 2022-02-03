#ifndef YASE_NEURON_H
#define YASE_NEURON_H

#include "yase.hh"

namespace yase {

    class Neuron : public Module {

    public:

      Neuron();
      void init();
      void update();

    private:

    };

}

#endif
