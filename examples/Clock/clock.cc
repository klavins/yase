#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Clock clock;
    Audio audio(3);
    MidiInput midi("MIDI Mix");
    Controls controls;
    Container container;
    
    clock.set_input("frequency", 4);

    container.add(clock)
             .add(midi)
             .add(audio)
             .add(controls)
             .connect(clock,"signal",audio,"aux1")
             .propagate_to(controls);

    controls.control(clock, "frequency", 1, 10, 60);

    container.run(UNTIL_INTERRUPTED);

    return 0;

}