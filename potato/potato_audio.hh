#ifndef YASE_POTATO_AUDIO_H
#define YASE_POTATO_AUDIO_H

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include "module.hh"

#define POTATO_NUM_SAMPLES 5000
#define POTATO_BUFFER_SIZE 10000

namespace yase {

    class PotatoAudio : public Module {

    public:

      PotatoAudio();
      void init();
      void update();

    private:

      int left, right;
      snd_output_t * output;
      snd_pcm_t * handle;

      float buffer[POTATO_BUFFER_SIZE];
      int n, k;

    };

}

#endif
