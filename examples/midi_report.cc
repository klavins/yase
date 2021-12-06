#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Synthesizer synth;
    Midi midi;

    synth.add(midi)
         .listen(MIDI_ANY, [](const Event &e) {
             std::cout << e.code << ", " 
                       << e.id << ", "
                       << e.value << "\n";
         });
    synth.run(-1);

    return 0;

}

