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
    //! If you re lucky enough to have an audio interface for more than four channels,
    //! the names of the extra channels will continue with aux3, aux4, etc.
    //! 
    class Audio : public Module {

    public:

      Audio(int num_output_channels=2, int num_input_channels=0);
      void init();
      void update();

    private:

      PaStreamParameters outputParameters, inputParameters;
      PaStream *stream;
      PaError err;
     
      int frame;
      int left, right; 
      int num_output_channels,
          num_input_channels;

      float * output_buffer, * input_buffer;

    };

}

#endif
