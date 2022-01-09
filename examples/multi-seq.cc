#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "yase.hh"
#include "mono.hh"

using namespace yase;
using namespace nlohmann;

int main(int argc, char * argv[]) {

    // Components
    Synthesizer synth("MIDI Mix"); 
    Midi midi;

    std::ifstream config_stream("config/akai-monophonic.json");
    json config, midi_map;
    config_stream >> config; 
    midi_map = config["midi_ids"];
    int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
    int controller_port = midi.get_port_id("MIDI Mix");
    string button_device_name = "MIDI Mix";

    Audio audio;  
    Mixer mixer(4);  
    Mono mono[]{
         Mono(midi, midi_map, button_device_name, keyboard_port, controller_port), 
         Mono(midi, midi_map, button_device_name, keyboard_port, controller_port), 
         Mono(midi, midi_map, button_device_name, keyboard_port, controller_port), 
         Mono(midi, midi_map, button_device_name, keyboard_port, controller_port)
    };

    synth.add(midi).add(audio).add(mixer);

    for ( int i=0; i<4; i++ ) {
         synth.add(mono[i]);
         synth.connect(mono[i], "signal", mixer, i);
    }

    synth.connect(mixer, "signal", audio, "left");
    synth.connect(mixer, "signal", audio, "right"); 

    // Select synth
    int current = 0;

    synth.mutex({15,18,21,24}, {
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
    synth.clear_leds();
    for ( auto s : mono ) {
         mono->clear_leds();
    }

    return 0;

}