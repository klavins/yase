#ifndef YASE_FADER_H
#define YASE_FADER_H

#include "module.hh"

namespace yase {

    class Fader : public Module {

    public:

      Fader();
      Fader(double min, double max);
      void init();
      void update();
      double adjusted_target();
      void randomize();

    private:

      float min_val,
            max_val;

      int target, value;

    };

}

#endif
