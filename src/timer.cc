#include "timer.hh"
#include "yase.hh"

namespace yase {


  Timer::Timer() : Module(), active(false) {

  }

  void Timer::set(float t, function<void()> f) {
    duration = t;
    handler = f;
    active = true;
    timer = 0;
  }

  void Timer::reset() {
    timer = 0;
    active = true;
  }

  void Timer::init() {

  }

  void Timer::update() {

    timer += TS;

    if ( timer > duration ) {
        active = false;      
        handler();
    }

  }    

}
