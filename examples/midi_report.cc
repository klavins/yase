#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Container synth;
    MidiInput midi(argv[1]);
    Audio audio; // for throttling

    synth.add(midi).add(audio);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}

