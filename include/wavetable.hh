#ifndef YASE_WAVETABLE_H
#define YASE_WAVETABLE_H

#include "yase.hh"
#include "AudioFile.h"

namespace yase {

    class Wavetable : public Oscillator {

    public:

      Wavetable(string path);
      void update();
      double value(int wave_num, double x);

    private:

      int length, num_waves;
      AudioFile<double> audioFile;
      int amplitude, morph, signal;

    };

}

#endif
