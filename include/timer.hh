#ifndef YASE_TIMER_H
#define YASE_TIMER_H

#include "yase.hh"

namespace yase {

    class Timer : public Module {

    public:

      Timer();
      void set(float t, function<void()> f);
      void reset();
      void init();
      void update();

    private:

      function<void()> handler;
      float duration;
      bool active;
      float timer;

    };

}

#endif
