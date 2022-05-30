#include <iostream>
#include <ncurses.h>
#include "yase.hh"

using namespace yase;

const int SIZE=20;
const int ids[4] = { 74, 71, 18, 19 };

class Report : public Module {

public:

  Report() {
       for ( int i=0; i<4; i++ ) {
         add_input("signal" + to_string(i));
       }
       buf[SIZE-1] = 0;
  }

  void init() {
       count = 0;
  }

  void update() {
       if ( count++ > 1000 ) {
            count = 0;
            for ( int i=0; i<4; i++ ) {
               u = inputs[i];
               for ( int j=0; j<SIZE-1; j++ ) {
                    buf[j] = j < (int) u ? '*' : ' ';
               }
               mvprintw(3*(i/2)+1, 2 + (SIZE+5) * (i%2), "Knob %d: %0.2f", i, u);
               mvprintw(3*(i/2)+2, 2 + (SIZE+5) * (i%2), "[%s]", buf);

            }            
            refresh();
       }
  }

  int signal;
  char buf[SIZE];
  double u;
  int count;

};

int main(int argc, char * argv[]) {

    Container synth;
    Controls controls;
    Audio audio;
    Report report;
    MidiInput midi("Arturia MiniLab mkII");

    synth.add(audio)
         .add(midi)
         .add(controls)
         .propagate_to(controls);

    controls.control(report, 0, 0, SIZE, ids[0])
            .linear();

    controls.control(report, 1, 0, SIZE, ids[1])
            .exponential(1e-2);

    controls.control(report, 2, 0, SIZE, ids[2])
            .exponential(1e2);

    controls.control(report, 3, 0, SIZE, ids[3])
            .exponential(1e5);

    initscr();    
    curs_set(0);
    synth.run(UNTIL_INTERRUPTED);
    endwin();

    return 0;

}