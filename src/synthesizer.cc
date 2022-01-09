#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "yase.hh"

#define SOURCE(__wire__) std::get<0>(__wire__)
#define OUTPUT(__wire__) std::get<1>(__wire__)
#define DEST(__wire__)   std::get<2>(__wire__)
#define INPUT(__wire__)  std::get<3>(__wire__)

bool interuppted = false;

void sighandler(int sig) {
	  interuppted = true;
}

namespace yase {

  Synthesizer::Synthesizer(string button_device_name) : ButtonManager(button_device_name) {

  }

  Synthesizer &Synthesizer::add(Module &module) {
    modules.push_back(&module);
    return *this;
  }

  Synthesizer::~Synthesizer() {
    for (auto f : faders) {
       delete f;
     } 
     faders.clear();    
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

  Synthesizer &Synthesizer::connect(Module &source, string output, Module &dest, int input) {
      
    Wire wire = {
      source,
      source.get_output_index(output),
      dest,
      input
    };

    wires.push_back(wire);

    return *this;

  }  

  Synthesizer &Synthesizer::disconnect(Module &source, string output, Module &dest, string input) {
    // TODO
    return *this;

  }

  void Synthesizer::init() {

    // call all init methods
    for(Module * m : modules) {
      m->init();
    }    

  }

  void Synthesizer::update() {

    // UPDATE BUTTON MANAGER
    ButtonManager::update();

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

  void Synthesizer::run(int num_steps) {

    signal(SIGINT, sighandler);

    init();

    // loop
    for(int i=0; !interuppted && ( num_steps < 0 || i<num_steps ); i++ ) {
      update();
    }
    
  } 

  Synthesizer &Synthesizer::control(Module &fader, int midi_id) {
    listeners[MIDI_MOD].push_back([&fader, midi_id] (Event e) {
      if ( e.id == midi_id ) {
        fader.set_input(0, e.value);
      }
    });
    return *this;
  }

  Synthesizer &Synthesizer::control(Module &module, string name, double min, double max, int midi_id) {

    Fader * fader = new Fader(min, max);
    add(*fader);
    connect(*fader, "value", module, name);
    control(*fader, midi_id);
    faders.push_back(fader);
    return *this;

  }

  Synthesizer &Synthesizer::control(Module &module, int index, double min, double max, int midi_id) {

    Fader * fader = new Fader(min, max);
    add(*fader);
    connect(*fader, "value", module, index);
    control(*fader, midi_id);
    faders.push_back(fader);
    return *this;

  }    

  void Synthesizer::randomize_faders() {

    for ( auto f : faders ) {
      f->randomize();
    }

  }

}
