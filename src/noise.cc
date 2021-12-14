#include "yase.hh"

namespace yase {

  Noise::Noise() : Oscillator(), distribution(0,1) {}

  void Noise::update() {

    outputs[signal] = distribution(generator);

  }    

}

