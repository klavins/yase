#include <string>
#include "sum.hh"
#include "yase.hh"

namespace yase {

  Sum::Sum(int n) : size(n) {

    for (int i=0; i<size; i++ ) {
      std::string name = "signal_" + std::to_string(i);
      add_input(name);
    }

    signal = add_output("signal");

  }

  Sum::Sum() : Sum(2) {}

  void Sum::init() {

  }

  void Sum::update() {
    outputs[signal] = 0;
    for ( int i=0; i<size; i++ ) 
      outputs[signal] += inputs[i];
  }    

}

