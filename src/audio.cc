#include <iostream>
#include "audio.hh"
#include "yase.hh"

namespace yase {

  Audio::Audio() {

      left = add_input("left");
      right = add_input("right");

  }

  void Audio::init() {

    err = Pa_Initialize();
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = 0.00;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,
              NULL,
              NULL);

    err = Pa_StartStream( stream );  // TODO: Check err

    frame = 0;

  }

  void Audio::update() {

      if ( frame >= FRAMES_PER_BUFFER ) {
        err = Pa_WriteStream( stream, buffer, FRAMES_PER_BUFFER );
        frame = 0;
      }    

      buffer[frame][0] = (float) inputs[left];
      buffer[frame][1] = (float) inputs[right];
      frame++;



  }    

}

