#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    if ( argc != 2 ) {
        std::cout << "Usage: midi_report device_name\n";
        exit(1);
    }    

    Container synth;
    MidiInput midi(argv[1]);
    Audio audio;

    synth.add(midi).add(audio);

    synth.listen(MIDI_ANY, [] (const Event &e) {
        std::cout << e.code << ", " << e.id << ", " << e.value << "\n";
    });

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}

