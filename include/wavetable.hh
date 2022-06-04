#ifndef YASE_WAVETABLE_H
#define YASE_WAVETABLE_H

#include "yase.hh"
#include "AudioFile.h"

namespace yase {

    //! A wavetable oscillator. 
    
    //! The path argument to the constructor
    //! should point to a .wav file that has 64 sections of 1024 samples. 
    //! Each section should be one cycle of the oscillator. You can use the
    //! morph input (in [0,1]) to choose the wave to play. The oscillator will interpolate
    //! between waves. For example, inputing w=0.1482 result in the following linear combination
    //! of waves floor(64*w) = 9 and ceiling(64*w) = 10:
    //!
    //! (1-64*w) * 9 + 64*w*10
    //!
    //! Example wavetables of the correct format can be found in the
    //! `wavetables` directory.
    //!
    //! \param[in] morph A value in the interval [0,1]
    //! \param[in] amplitude
    //! \param[out] signal
    //!
    class Wavetable : public Oscillator {

    public:

      Wavetable(string path);
      void update();
      
    private:

      double value(int wave_num, double x);

      int length, num_waves;
      AudioFile<double> audioFile;
      int amplitude, morph, signal;

    };

}

#endif
