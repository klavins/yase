// 
// YASE Audio Module Header
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

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

      bool initialized;

    };

}

#endif
