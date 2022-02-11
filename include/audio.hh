#ifndef YASE_AUDIO_H
#define YASE_AUDIO_H

#include "portaudio.h"
#include "globals.hh"
#include "module.hh"

namespace yase {

     //! 
     //! A module wrapper for a stereo PortAudio interface (http://www.portaudio.com/). 
     //! Use this module if you want to hear something coming from your speakers. By 
     //! default it uses the computer's default audio interface (whatever is set up in the
     //! system settings). The module accumulates FRAMES_PER_BUFFER input samples into a 2D array and
     //! then waits for the audio interface to be ready to receive them. Using this
     //! module in a project thus serves as an overall clock. 
     //! 
     //! \param[in] left
     //! \param[in] right
     //! \param[in] aux1
     //! \param[in] aux2     
     //! 
    class Audio : public Module {

    public:

      Audio(int num_channels=2);
      void init();
      void update();

    private:

      PaStreamParameters outputParameters;
      PaStream *stream;
      PaError err;

      float buffer[FRAMES_PER_BUFFER][2]; 
      
      int frame;
      int left, right, aux1, aux2; // indices into incoming signals
      int num_channels;

    };

}

#endif
