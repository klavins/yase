#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Synthesizer synth("MIDI Mix");
    Midi midi;

    synth.add(midi);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}

