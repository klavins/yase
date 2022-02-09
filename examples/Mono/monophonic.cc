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

     Container synth;
     Audio audio;
     MidiInput keyboard(config["keyboard"]),
               controller(config["controller"]);

     Mono mono(keyboard, controller, config);

     synth.add(mono)
          .add(audio)
          .add(keyboard)
          .add(controller)
          .propagate_to(mono);

     synth.connect(mono, "signal", audio, "left")
          .connect(mono, "signal", audio, "right");

     // Go!
     synth.run(UNTIL_INTERRUPTED);

     // Shutdown
     mono.clear_leds();
     return 0;

}