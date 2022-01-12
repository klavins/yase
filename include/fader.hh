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
      inline void set_tracking_gain(double x) { tracking_gain = x; }

    private:

      double min_val,
             max_val,
             tracking_gain;

      int target, value;

    };

}

#endif
