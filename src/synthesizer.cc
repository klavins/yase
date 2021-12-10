#include <iostream>
#include "yase.hh"

#define SOURCE(__wire__) std::get<0>(__wire__)
#define OUTPUT(__wire__) std::get<1>(__wire__)
#define DEST(__wire__)   std::get<2>(__wire__)
#define INPUT(__wire__)  std::get<3>(__wire__)

namespace yase {

  Synthesizer::Synthesizer() {

  }

  Synthesizer &Synthesizer::add(Module &module) {
    modules.push_back(&module);
    return *this;
  }

  Synthesizer &Synthesizer::connect(Module &source, string output, Module &dest, string input) {
      
    Wire wire = {
      source,
      source.get_output_index(output),
      dest,
      dest.get_input_index(input)
    };

    wires.push_back(wire);

    return *this;

  }

  Synthesizer &Synthesizer::disconnect(Module &source, string output, Module &dest, string input) {
    // TODO
    return *this;

  }

  void Synthesizer::run(int num_steps) {

    // call all init methods
    for(Module * m : modules) {
      m->init();
    }
    // loop
    for(int i=0; num_steps < 0 || i<num_steps; i++) {

      // WIRES
      for(Wire & w : wires) {
        DEST(w).set_input(INPUT(w), SOURCE(w).get_output(OUTPUT(w)));
      }

      // MODULES
      for(Module * m : modules) {
        m->update();
      }

      // EVENTS
      for(Module * m : modules ) {
        for(Event &event : m->events) {
            for(auto handler : listeners[event.code]) {
              handler(event);
            }
            for(auto handler : listeners[MIDI_ANY]) {
              handler(event);
            }            
        }
        m->events.clear();
      }

    }
    
  } 

  Synthesizer &Synthesizer::listen(int event_type, std::function<void(const Event &)> handler) {
    listeners[event_type].push_back(handler);
    return *this;
  }

  Synthesizer &Synthesizer::control(Module &fader, string input_name, int midi_id) {
    int i = fader.get_input_index(input_name);
    listeners[MIDI_MOD].push_back([&fader, i, midi_id] (Event e) {
      if ( e.id == midi_id ) {
        fader.set_input(i, e.value);
      }
    });
    return *this;
  }

}

