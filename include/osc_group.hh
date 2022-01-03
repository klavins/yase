#ifndef YASE_OSCGROUP_H
#define YASE_OSCGROUP_H

#include <vector>
#include "yase.hh"

namespace yase {

    class OscGroup : public Oscillator {

    public:

      OscGroup();
      ~OscGroup();
      void update();
      void select(float x);

    private:

      vector<Oscillator *> oscillators;
      int current;

    };

}

#endif