#include <iostream>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "yase.hh"
#include "mono.hh"

using namespace yase;
using namespace nlohmann;

int main(int argc, char * argv[]) {

    json config = get_config("config/akai-monophonic.json");

    // Components
    Container synth;
    ButtonManager buttons(config["controller"]); 
    Midi midi;
   
    int keyboard_port = midi.get_port_id(config["keyboard"]);
    int controller_port = midi.get_port_id(config["controller"]);

    Audio audio;  
    Mixer mixer(4);  
    Mono mono[]{
         Mono(midi, config["ids"], config["controller"], keyboard_port, controller_port), 
         Mono(midi, config["ids"], config["controller"], keyboard_port, controller_port), 
         Mono(midi, config["ids"], config["controller"], keyboard_port, controller_port), 
         Mono(midi, config["ids"], config["controller"], keyboard_port, controller_port)
    };

    synth.add(midi)
         .add(audio)
         .add(mixer)
         .add(buttons)
         .propagate_to(buttons);

    for ( int i=0; i<4; i++ ) {
         synth.add(mono[i]);
         synth.connect(mono[i], "signal", mixer, i);
    }

    synth.connect(mixer, "signal", audio, "left");
    synth.connect(mixer, "signal", audio, "right"); 

    // Select synth
    int current = 0;

    buttons.mutex({15,18,21,24}, {
        [&] (const Event &e) { current = 0; },
        [&] (const Event &e) { current = 1; },
        [&] (const Event &e) { current = 2; },
        [&] (const Event &e) { current = 3; }
    });

    synth.listen(MIDI_ANY, [&] (const Event &e) {
         mono[current].inject(e); 
    });   

    // Go!
    synth.run(UNTIL_INTERRUPTED);

    // Shutdown
    buttons.clear_leds();
    for ( auto s : mono ) {
         mono->clear_leds();
    }

    return 0;

}