#include <string>
#include "summer.hh"
#include "yase.hh"

namespace yase {

  Summer::Summer(int n) : size(n) {

    for (int i=0; i<size; i++ ) {
      std::string name = "signal_" + std::to_string(i);
      add_input(name);
    }

    signal = add_output("signal");

  }

  void Summer::init() {

  }

  void Summer::update() {
    outputs[signal] = 0;
    for ( int i=0; i<size; i++ ) 
      outputs[signal] += inputs[i];
  }    

}

