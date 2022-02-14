#include <iostream>
#include "audio.hh"
#include "yase.hh"

namespace yase {

  Audio::Audio(int num_channels) : num_channels(4) {

      left = add_input("left");
      right = add_input("right");
      aux1 = add_input("aux1");
      aux2 = add_input("aux2");

      set_input(left,0);
      set_input(right,0);
      set_input(aux1,0);
      set_input(aux2,0);

  }

  void Audio::init() {

    err = Pa_Initialize();
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = num_channels;
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

      if ( num_channels >= 3 ) {
        buffer[frame][2] = (float) inputs[aux1];
      }
      if ( num_channels >= 4 ) {
        buffer[frame][3] = (float) inputs[aux2];      
      }

      frame++;

  }    

}