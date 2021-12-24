#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Saw osc1;
    Saw osc2;
    Fader fader1(55,110), fader2(55,110);
    Audio audio;
    Midi midi;
    Synthesizer synth;

    osc1.set_input("frequency", 55);
    osc2.set_input("frequency", 55);

    synth.add(osc1)
         .add(osc2)
         .add(fader1)
         .add(fader2)
         .add(audio)
         .add(midi);

    synth.connect(fader1,"value",osc1,"frequency")
         .connect(fader2,"value",osc2,"frequency")
         .connect(osc1,"signal",audio,"left")
         .connect(osc2,"signal",audio,"right");

    synth.control(fader1, 19)
         .control(fader2, 23);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}

