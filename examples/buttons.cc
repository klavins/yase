#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Midi midi;
    Audio audio; // need to throttle execution
    Synthesizer synth;
    ButtonManager buttons("MIDI Mix");

    synth.add(midi)
         .add(audio)
         .add(buttons)
         .propagate_to(buttons);

    buttons.momentary(1, [&] (const Event &e) {
        std::cout << "1\n";
        buttons.blink_on(12, 0.5);
    })
    .momentary(3, [&] (const Event &e) {
        std::cout << "3\n";
        buttons.blink_off(12);
    });    

    buttons.mutex({4,7,6,9}, {
      [] (const Event &e) { std::cout << "4\n"; },
      [] (const Event &e) { std::cout << "7\n"; },
      [] (const Event &e) { std::cout << "6\n"; },
      [] (const Event &e) { std::cout << "9\n"; }
    });

    buttons.blink_on(10, 0.25);

    synth.run(UNTIL_INTERRUPTED);

    buttons.clear_leds(); 

    return 0;

}