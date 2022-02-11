#include "timer.hh"
#include "yase.hh"

namespace yase {


  Timer::Timer() : Module(), active(false) {

  }

  //! Set the duration of a timer and define what to do when the
  //! time is up.
  //! \param t The duration, in seconds
  //! \param f A method, for example expressed as a lambda
  void Timer::set(float t, function<void()> f) {
    duration = t;
    handler = f;
    active = true;
    timer = 0;
  }

  //! Reset the timer. Calling set on a timer only triggers its method once.
  //! If you want it to trigger again (with the same associated method) cal reset.
  void Timer::reset() {
    timer = 0;
    active = true;
  }

  void Timer::init() {

  }

  void Timer::update() {

    timer += TS;

    if ( active && timer > duration ) {
        active = false;      
        handler();
    }

  }    

}
