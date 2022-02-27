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
