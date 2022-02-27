#ifndef YASE_MIXER_H
#define YASE_MIXER_H

#include <vector>
#include "gain.hh"
#include "sum.hh"

namespace yase {

    class Mixer : public Module {

    public:

      Mixer(int n);
      void init();
      void update();

      inline int amplitude_index(int i) { return n + i; }
      void set_amplitude_input(int i, double value); 
      double get_amplitude_input(int i);

    private:

      int n, 
          signal, 
          output_gain;

    };

}

#endif
