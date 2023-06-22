// 
// YASE Sampler Module Implementation
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

#include "sampler.hh"
#include "yase.hh"
#include "AudioFile.h"

namespace yase {

  //! Construct a Sampler Module
  //! \param path The path to write the resulting wav file to
  Sampler::Sampler(string path) : path(path), index(0), active(false) {

    signal = add_input("signal");
    buffers.push_back(new double[BUFFER_LENGTH]);

  }

  void Sampler::init() {

  }

  void Sampler::update() {
    if ( active ) {
      if ( index < BUFFER_LENGTH ) {
        buffers.back()[index++] = inputs[signal];
      } else {
        index = 0;
        buffers.push_back(new double[BUFFER_LENGTH]);      
      }
    }
  }    

  //! Turn on the Sampler so that it records its input
  Sampler &Sampler::record() {

    active = true;
    return *this;

  }

  //! Turn off the Sample so that it stops recording
  Sampler &Sampler::stop() {

    active = false;
    return *this;

  }

  //! Save the recorded signal to the file specified by the `path` parameter when the 
  //! Sampler Module was constructed. 
  Sampler &Sampler::write() {

    std::cout << "Recorded " << buffers.size() * BUFFER_LENGTH + index << " samples\n";

    AudioFile<double> audioFile;
    AudioFile<double>::AudioBuffer abuf;

    abuf.resize(1);
    abuf[0].resize(buffers.size() * BUFFER_LENGTH + index);

    for ( int i = 0; i< buffers.size(); i++) {
      for ( int j = 0; j<BUFFER_LENGTH; j++ ) {
        if ( i < buffers.size() - 1 || j < index ) {
          abuf[0][i*BUFFER_LENGTH + j] = buffers[i][j];
        }
      }
    }

    audioFile.setAudioBuffer(abuf);

    audioFile.save(path);

    return *this;

  }

}

