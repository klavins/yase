#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Synthesizer synth;
    Midi midi;

    synth.add(midi)
         .listen("midi_event", [](Event * event) {
             MidiEvent * me = (MidiEvent *) event;
             std::cout << me->code << ", " 
                       << me->id << ", "
                       << me->value << "\n";
         });
    synth.run(-1);

    return 0;

}

