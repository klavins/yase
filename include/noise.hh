#ifndef YASE_NOISE_H
#define YASE_NOISE_H

#include <random>
#include "yase.hh"

namespace yase {

    class Noise : public Oscillator {

    public:

      Noise();
      void update();

    private:

      std::normal_distribution<double> distribution;
      std::default_random_engine generator;

    };

}

#endif
