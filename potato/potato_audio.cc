#include "potato_audio.hh"
#include "yase.hh"

namespace yase {

  PotatoAudio::PotatoAudio() : output(NULL), n(0) {

    left = add_input("left");
    right = add_input("right");

    int err;

    if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
            printf("Playback open error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_set_params(handle,
                                  SND_PCM_FORMAT_FLOAT,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  2, // channels
                                  SAMPLE_RATE,
                                  1,
                                  500000)) < 0) {
            printf("Playback open error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);


    }

  }

  void PotatoAudio::init() { }

  void PotatoAudio::update() {

    buffer[n++] = inputs[left];
    buffer[n++] = inputs[right];
    if ( n >= POTATO_BUFFER_SIZE ) {
        snd_pcm_writei(handle, buffer, 1000);
        n = 0;
    }

  }

}
