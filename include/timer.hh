#ifndef YASE_TIMER_H
#define YASE_TIMER_H

#include "yase.hh"

namespace yase {

    //! A timer that can be used to do something one or more times in the future.
    //! For example, the following prints "Time's up!" every second.
    //! \code{cpp}
    //! Timer timer;
    //! Container container;
    //! container.add(timer)
    //! timer.set(1, [&]() {
    //!     std::cout << "Time's up!";
    //!     timer.reset();
    //! });
    //! \endcode
    //!
    //! This module has no inputs or outputs
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
