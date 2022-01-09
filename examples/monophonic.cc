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
     Audio audio;
     Midi midi;

     // Load config file
     std::ifstream config_stream("config/akai-monophonic.json");
     json config, midi_map;
     config_stream >> config; 
     midi_map = config["midi_ids"];
     int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
     int controller_port = midi.get_port_id("MIDI Mix");
     string button_device_name = "MIDI Mix";

     Mono mono(midi, midi_map, "MIDI Mix", keyboard_port, controller_port);

     synth.add(mono)
          .add(audio)
          .add(midi);

     synth.connect(mono, "signal", audio, "left")
          .connect(mono, "signal", audio, "right");

     synth.listen(MIDI_ANY, [&] (const Event &e) {
          mono.inject(e); 
     });   

     // Go!
     synth.run(UNTIL_INTERRUPTED);

     // Shutdown
     synth.clear_leds();
     mono.clear_leds();
     return 0;

}