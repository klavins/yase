// 
// YASE Fadeable Delay Module Implementation
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

#include <math.h>
#include "fadable_delay.hh"
#include "yase.hh"

namespace yase {

  FadableDelay::FadableDelay() 
      : state(NORMAL), 
        fade(1.0),
        counter(0),
        delays{Delay(SAMPLE_RATE), 
               Delay(SAMPLE_RATE)} {

    signal = add_input("signal");
    signal = add_output("signal");
    duration = add_input("duration");

    current_duration = SAMPLE_RATE;
    current = 0;

    add(delays[0]);
    add(delays[1]);

  }

  void FadableDelay::init() {

  }

  void FadableDelay::update() {

    if ( counter++ == 1000 ) {
      counter = 0;
    }

    if ( state == NORMAL && current_duration != floor(inputs[duration]) ) {
      current_duration = floor(inputs[duration]);
      current = 1 - current;
      delays[current].set(current_duration);
      fade = 0;
      state = RECORDING;
    } else if ( state == RECORDING && delays[current].is_full() ) {
      state = FADING;
    } else if ( state == FADING && fade < 1.0 ) {
      fade += ts / FADE_TIME;
    } else if ( state == FADING && fade >= 1.0 ) {
      fade = 1.0;
      state = NORMAL;
    }
    
    delays[0].set_input(signal,inputs[signal]);
    delays[1].set_input(signal,inputs[signal]);
    Container::update();
    outputs[signal] = fade * delays[current].get_output(signal) 
                    + (1-fade) * delays[1-current].get_output(signal);

  }    

}

