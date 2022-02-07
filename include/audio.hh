#ifndef YASE_AUDIO_H
#define YASE_AUDIO_H

#include "portaudio.h"
#include "globals.hh"
#include "module.hh"

namespace yase {

     //! A module wrapper for a stereo PortAudio interface (http://www.portaudio.com/). 
     //! Use this module if you want to hear something coming from your speakers. By 
     //! default it uses the computer's default audio interface (whatever is set up in the
     //! system settings). The module accumulates FRAMES_PER_BUFFER input samples into a 2D array and
     //! then waits for the audio interface to be ready to receive them. Using this
     //! module in a project thus serves as an overall clock. 
     //! 
     //! Inputs: 
     //!   - "left"
     //!   - "right"
     //! 
     //! Outputs: None
     //! 
    class Audio : public Module {

    public:

      Audio();
      void init();
      void update();

    private:

      PaStreamParameters outputParameters;
      PaStream *stream;
      PaError err;    

      float buffer[FRAMES_PER_BUFFER][4]; /* stereo output buffer */
      int frame;

      int left, right, aux1, aux2; // indecies into incoming signals

    };

}

#endif
