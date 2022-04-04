#ifndef YASE_CYCLE_H
#define YASE_CYCLE_H

#include "yase.hh"

namespace yase {

    //! Cycle through a list of pitches, running a callback for each one with the
    //! specified duration.
    class Cycle : public Module {

    public:

      Cycle();
      void set(vector<double> pitch_list, function<void(double)> f, double dt);
      void init();
      void update();

    private:

      function<void(double)> callback;
      double duration, t;
      vector<double> pitches;
      int n;

    };

}

#endif
