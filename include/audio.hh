#ifndef YASE_AUDIO_H
#define YASE_AUDIO_H

#include "portaudio.h"
#include "yase.hh"

namespace yase {

    class Audio : public Module {

    public:

      Audio();
      void init();
      void update();

    private:

      PaStreamParameters outputParameters;
      PaStream *stream = NULL;
      PaError err;    

      float buffer[FRAMES_PER_BUFFER][2]; /* stereo output buffer */
      int frame;

      int left, right; // indecies into incoming signals

    };

}

#endif
