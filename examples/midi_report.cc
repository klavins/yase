#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Synthesizer synth;
    Midi midi;
    Audio audio; // for throttling

    synth.add(midi).add(audio);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}

