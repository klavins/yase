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

#include "yase.hh"
#include <ncurses.h>

using namespace yase;

// This example is derived from the Sine example, but also shows how to respond to
// external events by polling for whether data is ready and then doing something.
// Note that if you want to use Midi events, that's already built into the EventManager 
// class. This example, and the Monitor class in particular, is for responding to 
// other types of events, such as the arrival of serial data. 
//
// The particular event used for this example is a (alpha-numeric) keyboard press. The
// easy way to check for a key press is with the ncurses library function wgetch, which 
// returns ERR if there is no character read in. 

int main(int argc, char * argv[]) {

    Sine sine1, sine2;
    Audio audio;
    Container synth;

    Monitor monitor;           // Yase class for monitor non-MIDI events
    WINDOW *w = initscr();     // An ncurses window, needed to make wgetch work
    timeout(0);                // Set ncurses to non-blocking
    noecho();                  // Turn off echoing of key presses
    char c;                    // A place to put the characters returned by wgetch

    sine1.set_input("frequency", A4);
    sine2.set_input("frequency", A4+1);

    monitor.listen(
    [&] () {                  // Condition to check
        c = wgetch(w); 
        return c != ERR;
     }, 
     [&] () {                 // What to do when the condition is true
      sine1.set_input("frequency", B4);
      sine2.set_input("frequency", B4+1);
    });

    synth.connect(sine1,"signal",audio,"left")
         .connect(sine2,"signal",audio,"right")
         .add(monitor);       // Don't forget to add the monitor to the main container

    synth.run(UNTIL_INTERRUPTED);

    endwin();                 // Turn off the ncurses window manager

    return 0; 

}