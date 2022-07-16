// 
// YASE Example
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
#include <math.h>
#include "yase.hh"

using namespace yase;

class TappedDelay : public Module {

  public:

  TappedDelay(double duration) : Module(), duration(duration), buffer_index(0) {
    duration_in_samples = ceil(duration * SAMPLE_RATE);
    buffer = new double[duration_in_samples];
    signal = add_input("signal");
  }

  // Here you would do something like
  //   auto tap = add_tap();
  //   synth.connect(lfo, "signal", tapped_delay, tap);
  //   synth.connect(tapped_delay, tap, audio, "left");
  string add_tap() {
    int index = inputs.size();
    string tap_name = "tap_" + std::to_string(index);
    add_input(tap_name);
    add_output(tap_name);
    return tap_name;
  }

  void init() {
    for(int i=0;i<duration_in_samples;i++) {
      buffer[i] = 0.0;
    }
  }

  void update() {
    add_sample(inputs[signal]);
    for ( int i=0; i<inputs.size()-1; i++ ) {
      outputs[i] = get_value(tap_position(i));
    }
  }

  double tap_position(int i) {
    return inputs[i+1];
  }

  /*

              + ----- buffer_index
              |
              V
    . . . * * . . . . . . . . . . 
  
  */
  void add_sample(double u) {
    buffer[buffer_index] = u;
    buffer_index++;
    if ( buffer_index > duration_in_samples ) {
      buffer_index = 0;
    }
  }

  int fix(int n) {
    int m = n;
    while ( m < 0 ) {
      m += duration_in_samples;
    }
    while ( m >= duration_in_samples ) {
      m -= duration_in_samples;
    }
    return m;    
  }

  double get_value(double position) {
    int a = fix(buffer_index + floor(position * SAMPLE_RATE)),
        b = fix(buffer_index + ceil(position * SAMPLE_RATE ));
    double A = ( a - buffer_index ) / SAMPLE_RATE;
    return (buffer[b] - buffer[a])*(position-A) + buffer[a];
  }

  ~TappedDelay() {
    delete[] buffer;
  }

  private:

  double duration;
  int duration_in_samples;
  int signal;
  double * buffer;
  int buffer_index;

};

int main(int argc, char * argv[]) {

    double C = 0.001,    // tap center (seconds)
           A = 0.0001,   // tap position amplitude (seconds)
           F = 1.5;      // frequency (per second) 
    
    Container synth;
    Audio audio;
    Saw raw_saw("raw");
    Sine lfo;
    Transform scale([&](double u) { return A*u + C; });
    AntiAlias saw(raw_saw);
    TappedDelay delay(2*C);
    Envelope wave_envelope;
    Player player;
    Gain gain;
    Sum sum;

    auto tap = delay.add_tap();
    delay.set_input(tap, C);

    lfo.set_input("frequency", F);

    saw.set_input("amplitude", 0.4);
    gain.set_input("amplitude", 0.35);
    wave_envelope.set_adsr(0.005, 1.0, 1.0, 0.1);

    synth.connect(saw, delay)
         .connect(saw, "signal", sum, 0)
         .connect(delay, tap, sum, 1)
         .connect(sum, gain)
         .connect(gain, "signal", audio, "left")
         .connect(gain, "signal", audio, "right")
         .connect(wave_envelope, "signal", saw, "amplitude")
         .connect(lfo, scale)
         .connect(scale, "signal", delay, tap)
         .add(player);

    player.set({ C2,  REST, C2,  REST, AS1, REST, AS1, REST, 
                 GS1, REST, GS1, REST, G1,  REST, G1,  REST }, 
      [&] (double freq) {
        if ( freq != REST ) {
          saw.set_input("frequency", freq);
          wave_envelope.trigger();
        } else {
          wave_envelope.release();
        }
      }, 1.5);

    synth.run(UNTIL_INTERRUPTED);

    DEBUG
    return 0; 

}