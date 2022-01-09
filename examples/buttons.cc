#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Midi midi;
    Audio audio; // need to throttle execution
    Synthesizer synth("MIDI Mix");

    synth.add(midi)
         .add(audio);

    synth.momentary(1, [&] (const Event &e) {
        std::cout << "1\n";
        synth.blink_on(12, 0.5);
    })
    .momentary(3, [&] (const Event &e) {
        std::cout << "3\n";
        synth.blink_off(12);
    });    

    synth.mutex({4,7,6,9}, {
      [] (const Event &e) { std::cout << "4\n"; },
      [] (const Event &e) { std::cout << "7\n"; },
      [] (const Event &e) { std::cout << "6\n"; },
      [] (const Event &e) { std::cout << "9\n"; }
    });

    synth.blink_on(10, 0.25);

    synth.run(UNTIL_INTERRUPTED);

    synth.clear_leds(); 

    return 0;

}