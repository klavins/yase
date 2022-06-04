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
#include "yase.hh"

using namespace yase;

// https://d2xhy469pqj8rc.cloudfront.net/sites/default/files/novation/downloads/4080/launchpad-programmers-reference.pdf
// https://www.djshop.gr/Attachment/DownloadFile?downloadId=10737

int main(int argc, char * argv[]) {

    Audio audio;
    Buttons buttons("Launchpad Mini MK3 LPMiniMK3 MIDI In");
    Timer timer;
    Container synth;
    
    synth.add(buttons)
         .add(timer)
         .add(audio);

    buttons.send({240,0,32,41,2,13,0,127,247}); // set launchpad to programmer mode

    int n = 0;
    auto assign_colors = [&] () {
        int m = 0;
        for ( int row=1; row<9; row++ ) {
            for ( int col=1; col<9; col++ ) {
                buttons.on(10*row + col, 16+(n+m)%9);   
                m++;
            }
        }
        timer.reset();
        n++;
    };

    timer.set(0.0625, assign_colors);

    synth.run(UNTIL_INTERRUPTED);

    buttons.clear_leds();

    return 0;

}
