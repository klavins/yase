// 
// YASE Audio Module Implementation
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

#include <iostream>
#include "audio.hh"
#include "yase.hh"

namespace yase {

  Audio::Audio(int num_output_channels, int num_input_channels) : 
    num_output_channels(num_output_channels),
    num_input_channels(num_input_channels) {

      left = add_input("left");
      right = add_input("right");

      set_input(left,0);
      set_input(right,0);

      for ( int i=2; i<num_output_channels; i++ ) {
          int j = add_input("aux" + std::to_string(i-1));
          set_input(j, 0);
      }

      for ( int i=0; i<num_input_channels; i++ ) {
          int j = add_output("line_in" + std::to_string(i));
      }

      output_buffer = new float[FRAMES_PER_BUFFER * num_output_channels];       
      input_buffer = new float[FRAMES_PER_BUFFER * num_input_channels];       

  }

  void Audio::init() {

    err = Pa_Initialize();

    // SET UP AUDIO OUTPUTS
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = num_output_channels;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = 0.00;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    const PaDeviceInfo * info = Pa_GetDeviceInfo(outputParameters.device);

    std::cout << "\nUsing Audio Device '" << info->name << "'\n";
    std::cout << "  - Inputs: " << info->maxInputChannels << "\n";
    std::cout << "  - Outputs: " << info->maxOutputChannels << "\n";
    std::cout << "  - Outputs requested: " << num_output_channels << "\n";    
    std::cout << "  - Inputs requested: " << num_input_channels << "\n\n";       

    if ( num_output_channels > info->maxOutputChannels ) {
        throw Exception(
          "Audio device has fewer output channels (" + 
          std::to_string(info->maxOutputChannels) + 
          ") than requested (" +
          std::to_string(num_output_channels) + 
          ")"
        );
    }    

    // SET UP AUDIO INPUTS

    if ( num_input_channels > 0 ) {
      inputParameters.device = Pa_GetDefaultOutputDevice();
      inputParameters.channelCount = num_input_channels;
      inputParameters.sampleFormat = PA_SAMPLE_TYPE;
      inputParameters.suggestedLatency = 0.00;
      inputParameters.hostApiSpecificStreamInfo = NULL;

      if ( num_input_channels > info->maxInputChannels ) {
          throw Exception(
            "Audio device has fewer input channels (" + 
            std::to_string(info->maxInputChannels) + 
            ") than requested (" +
            std::to_string(num_input_channels) + 
            ")"
          );
      }
    }

    err = Pa_OpenStream(
              &stream,
              num_input_channels > 0 ? &inputParameters : NULL,
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
        err = Pa_WriteStream( stream, output_buffer, FRAMES_PER_BUFFER );
        // if ( err != paNoError ) {
        //     throw Exception(
        //       "Failed to write to audio stream with error: " + 
        //       std::string(Pa_GetErrorText(err))
        //     );
        // }
        if ( num_input_channels > 0 ) {
          err = Pa_ReadStream( stream, input_buffer, FRAMES_PER_BUFFER );
        }
        frame = 0;
      }

      output_buffer[frame * num_output_channels + 0] = (float) inputs[left];
      output_buffer[frame * num_output_channels + 1] = (float) inputs[right];

      for ( int i=2; i<num_output_channels; i++ ) {
          output_buffer[frame * num_output_channels + i] = (float) inputs[i];
      }

      for ( int i=0; i<num_input_channels; i++ ) {
        outputs[i] = (double) input_buffer[frame * num_input_channels + i];
      }

      frame++;

  }    
  
}