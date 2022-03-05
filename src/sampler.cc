#include "sampler.hh"
#include "yase.hh"
#include "AudioFile.h"

namespace yase {

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

  Sampler &Sampler::record() {

    active = true;
    return *this;

  }

  Sampler &Sampler::stop() {

    active = false;
    return *this;

  }

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

