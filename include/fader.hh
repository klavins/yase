#ifndef YASE_FADER_H
#define YASE_FADER_H

#include "yase.hh"

namespace yase {

    class Fader : public Module {

    public:

      Fader();
      Fader(double min, double max);
      Fader(double min, double max, bool inverted);
      void init();
      void update();
      double adjusted_target();

    private:

      float min_val,
            max_val;

      int target, value;

      bool inverted;

    };

}

#endif
