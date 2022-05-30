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
      inline void linear() { is_linear = true; }
      inline void exponential(double b) { is_linear = false; base = b; }  

    private:

      double min_val,
             max_val,
             tracking_gain,
             base;

      int target, value;

      bool is_linear;

    };

}

#endif
