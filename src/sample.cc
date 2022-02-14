#include "sample.hh"
#include "yase.hh"

namespace yase {

  //! Construct a new sample module from a .wav file
  //! \param path The path to a .wav file.
  Sample::Sample(string path) : Module(), active(false), n(0) {

    audioFile.load (path);
    if ( audioFile.getNumChannels() != 2 ) {
      throw Exception("Expected sample to have two channels");
    }
    length = audioFile.getNumSamplesPerChannel();
    n = length; // initially off

    left = add_output("left");
    right = add_output("right");
    amplitude = add_input("amplitude");

    inputs[amplitude] = 0.25;

  }

  //! Trigger the sample to play. It needs to have been added to a container first. 
  void Sample::trigger() {
    n = 0;
  }

  void Sample::init() {

  }

  void Sample::update() {
    if ( n < length ) {
      outputs[left] = inputs[amplitude] * audioFile.samples[0][n];
      outputs[right] = inputs[amplitude] * audioFile.samples[1][n]; 
      n++;
    } else {
      outputs[left] = 0;
      outputs[right] = 0;      
    }

  }    

  //! Stop playing the sample
  void Sample::stop() {
    n = length;
  }

}

