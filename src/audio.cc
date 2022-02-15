#include <iostream>
#include "audio.hh"
#include "yase.hh"

namespace yase {

  Audio::Audio(int num_channels) : num_channels(num_channels) {

      left = add_input("left");
      right = add_input("right");

      set_input(left,0);
      set_input(right,0);

      for ( int i=2; i<num_channels; i++ ) {
          int j = add_input("aux" + std::to_string(i-1));
          set_input(j, 0);
      }

      buffer = new float[FRAMES_PER_BUFFER * num_channels];       

  }

  void Audio::init() {

    err = Pa_Initialize();
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = num_channels;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = 0.00;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    const PaDeviceInfo * info = Pa_GetDeviceInfo(outputParameters.device);

    std::cout << "Using Audio Device '" << info->name << "'\n";
    std::cout << "  - Inputs: " << info->maxInputChannels << "\n";
    std::cout << "  - Outputs: " << info->maxOutputChannels << "\n";
    std::cout << "  - Outputs requested: " << num_channels << "\n";

    if ( num_channels > info->maxOutputChannels ) {
        throw Exception(
          "Audio device has fewer output channels (" + 
          std::to_string(info->maxOutputChannels) + 
          ") than requested (" +
          std::to_string(num_channels) + 
          ")"
        );
    }

    err = Pa_OpenStream(
              &stream,
              NULL,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,
              NULL,
              NULL);

    if ( err != paNoError ) {
        throw Exception(
          "Failed to open audio stream with error: " + 
          std::string(Pa_GetErrorText(err))
        );
    }    

    err = Pa_StartStream( stream );

    if ( err != paNoError ) {
        throw Exception(
          "Failed to start audio stream with error: " + 
          std::string(Pa_GetErrorText(err))
        );
    }

    frame = 0;

  }

  void Audio::update() {        

      if ( frame >= FRAMES_PER_BUFFER ) {
        err = Pa_WriteStream( stream, buffer, FRAMES_PER_BUFFER );
        if ( err != paNoError ) {
            throw Exception(
              "Failed to write to audio stream with error: " + 
              std::string(Pa_GetErrorText(err))
            );
        }
        frame = 0;
      }

      buffer[frame * num_channels + 0] = (float) inputs[left];
      buffer[frame * num_channels + 1] = (float) inputs[right];

      for ( int i=2; i<num_channels; i++ ) {
          buffer[frame * num_channels + i] = (float) inputs[i];
      }

      frame++;

  }    
  
  void Audio::show_buffer() {
    for ( int i=0; i<FRAMES_PER_BUFFER; i++  ) {
        if ( i == frame ) {
            std::cout << "----------------\n";
        }
        for ( int j=0; j<num_channels; j++ ) {
            std::cout << buffer[i*num_channels + j] << "\t";
        }
        std::cout << "\n";
    }
  }

}