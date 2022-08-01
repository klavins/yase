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
#include <ncurses.h>
#include "yase.hh"

using namespace yase;

const int SIZE=20;
const string device_name = "Arturia MiniLab mkII";
const int midi_ids[4] = { 74, 71, 18, 19 };

class Report : public Module {

public:

  Report() {
       for ( int i=0; i<4; i++ )
         add_input("signal_" + to_string(i));
       buf[SIZE-1] = 0;
  }

  void init() {
       count = 0;
  }

  void update() {
       if ( count++ > 1000 ) {
            count = 0;
            for ( int i=0; i<4; i++ ) {
               for ( int j=0; j<SIZE-1; j++ )
                    buf[j] = j < (int) inputs[i] ? '*' : ' ';
               mvprintw(3*(i/2)+1, 2 + (SIZE+5) * (i%2), "Knob %d: %0.2f", i, inputs[i]);
               mvprintw(3*(i/2)+2, 2 + (SIZE+5) * (i%2), "[%s]", buf);

            }            
            refresh();
       }
  }

  char buf[SIZE];
  int count;

};

int main(int argc, char * argv[]) {

    Container synth;
    Controls controls;
    Audio audio; // There is no sound, but the audio module regulates timing
    Report report;
    MidiInput midi(device_name);

    synth.add(audio)
         .add(midi)
         .add(controls)
         .propagate_to(controls);

    controls.map(report, 0, 0, SIZE, midi_ids[0]).linear();
    controls.map(report, 1, 0, SIZE, midi_ids[1]).exponential(1e-2);
    controls.map(report, 2, 0, SIZE, midi_ids[2]).exponential(1e2);
    controls.map(report, 3, 0, SIZE, midi_ids[3]).exponential(1e5);

    initscr();    
    curs_set(0);
    synth.run(UNTIL_INTERRUPTED);
    endwin();

    return 0;

}