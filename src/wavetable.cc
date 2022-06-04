// 
// YASE Wavetable Module Implementation
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

#include "yase.hh"

namespace yase {

  Wavetable::Wavetable(string path) : Oscillator() {

    if ( !audioFile.load (path) ) {
      throw Exception(std::string("Could not load wavetable ") + path);
    }

    int num_channels = audioFile.getNumChannels();

    if ( num_channels != 1 ) {
      std::cout << num_channels << "\n";
      throw Exception("Expected wavetable to have one channel");
    }

    length = audioFile.getNumSamplesPerChannel();
    num_waves = 64;
    length = length / num_waves;

    amplitude = add_input("amplitude");
    morph = add_input("morph");

    signal = add_output("signal");

    inputs[morph] = 0;
    inputs[amplitude] = 0.25;

  }

  double Wavetable::value(int wave_num, double x) {

    int x1 = floor(x),
        x2 = (x1+1) % length;

    double y1 = audioFile.samples[0][length * wave_num + x1],
           y2 = audioFile.samples[0][length * wave_num + x2],
           m = ( y2-y1 ) / ( x2 - x1 );

    if ( abs(m) < 0.0001 ) {  
        return y1;
    } else {
        return m * ( x - x1 ) + y1;
    }

  }

  int r = 0;

  void Wavetable::update() { 

    Oscillator::update();

    double x = phase * length,
           w = inputs[morph] * num_waves;

    int wave_num = (int) w;

    // if ( r++ > 10000 ) {
    //     std::cout << inputs[morph] << ", " << w << ", " << wave_num << "\n";
    //     r = 0;
    // }

    if ( wave_num <= 0 || wave_num >= num_waves - 1 ) {

      outputs[signal] = value(wave_num,x);

    } else {

      double v1 = value(wave_num, x),
             v2 = value(wave_num+1, x),
             f = w - wave_num; // should be the fractional part of w

      outputs[signal] = (1-f) * v1 + f * v2;

    }
    
  }    

}
