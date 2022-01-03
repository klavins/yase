#include <math.h>
#include "yase.hh"

namespace yase {

  OscGroup::OscGroup() : Oscillator() {

    oscillators.push_back(new Sine);
    oscillators.push_back(new Saw);
    oscillators.push_back(new Triangle);
    oscillators.push_back(new Square);
    oscillators.push_back(new Noise);

    current = 0;

  }

  void OscGroup::update() {

    oscillators[current]->copy_inputs(*this);
    oscillators[current]->update();
    oscillators[current]->copy_outputs(*this);

  }    

  void OscGroup::select(float x) {

    int i = floor(x * oscillators.size());

    if ( i >= 0 && i < oscillators.size() ) {
      current = i;
    }

  }

  OscGroup::~OscGroup() {
    for (auto o : oscillators) {
       delete o;
     } 
     oscillators.clear();
  }

}