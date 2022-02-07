#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Saw osc1;
    Saw osc2;
    Controls controls;
    Audio audio;
    MidiInput midi("MIDI Mix");
    Container synth;

    osc1.set_input("frequency", 55);
    osc2.set_input("frequency", 55);

    synth.add(osc1)
         .add(osc2)
         .add(audio)
         .add(midi)
         .add(controls)
         .propagate_to(controls);

    synth.connect(osc1,"signal",audio,"left")
         .connect(osc2,"signal",audio,"right");

    controls.control(osc1, "frequency", 55, 110, 19)
            .control(osc2, "frequency", 55, 110, 23);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}

