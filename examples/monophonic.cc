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

     json config = get_config("config/akai-monophonic.json");

     // Components
     Synthesizer synth(config["controller"]); 
     Audio audio;
     Midi midi;

     // Load config file
     int keyboard_port = midi.get_port_id(config["keyboard"]);
     int controller_port = midi.get_port_id(config["controller"]);

     Mono mono(midi, config["ids"], config["controller"], keyboard_port, controller_port);

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