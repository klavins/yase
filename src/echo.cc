/*! \file */

#include "echo.hh"
#include "yase.hh"

namespace yase {

  Echo::Echo() : Container(), sum(2) {

    signal = add_input("signal");
    signal = add_output("signal");
    amplitude = add_input("amplitude");
    duration = add_input("duration");

    add(sum);
    add(delay);
    add(gain);

    equate_input("signal", sum, "signal_0");
    equate_input("duration", delay, "duration");
    equate_input("amplitude", gain, "amplitude");

    equate_output("signal", sum, "signal");

    connect(gain, "signal", sum, 1);
    connect(sum, "signal", delay, "signal");
    connect(delay, "signal", gain, "signal");

  }

  void Echo::init() {
    Container::init();
  }

  void Echo::update() {
    Container::update();
  }    

}

