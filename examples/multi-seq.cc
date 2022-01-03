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
    Synthesizer synth; 
    Midi midi;

    std::ifstream config_stream("config/akai-monophonic.json");
    json config, midi_map;
    config_stream >> config; 
    midi_map = config["midi_ids"];
    int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
    int controller_port = midi.get_port_id("MIDI Mix");

    Audio audio;  
    Mixer mixer(4);  
    Mono mono[]{
         Mono(midi, midi_map, keyboard_port, controller_port), 
         Mono(midi, midi_map, keyboard_port, controller_port), 
         Mono(midi, midi_map, keyboard_port, controller_port), 
         Mono(midi, midi_map, keyboard_port, controller_port)
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
    midi.on(controller_port, 15);
    synth.button(controller_port, 15, [&] (const Event &e) {
         midi.on(controller_port, 15);
         midi.off(controller_port, 18);
         midi.off(controller_port, 21);
         midi.off(controller_port, 24);
         current = 0;
    });

    synth.button(controller_port, 18, [&] (const Event &e) {
         midi.off(controller_port, 15);
         midi.on(controller_port, 18);
         midi.off(controller_port, 21);
         midi.off(controller_port, 24);
         current = 1;
    });

    synth.button(controller_port, 21, [&] (const Event &e) {
         midi.off(controller_port, 15);
         midi.off(controller_port, 18);
         midi.on(controller_port, 21);
         midi.off(controller_port, 24);
         current = 2;
    });

    synth.button(controller_port, 24, [&] (const Event &e) {
         midi.off(controller_port, 15);
         midi.off(controller_port, 18);
         midi.off(controller_port, 21);
         midi.on(controller_port, 24);
         current = 3;
    });

    synth.listen(MIDI_ANY, [&] (const Event &e) {
         mono[current].inject(e); 
    });   

    // Go!
    synth.run(UNTIL_INTERRUPTED);

    // Shutdown
    for ( auto& [_, id] : midi_map["buttons"].items() ) {
      midi.off(controller_port, id);
    }

    return 0;

}