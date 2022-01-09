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

  Container::Container() {

  }

  Container &Container::add(Module &module) {
    modules.push_back(&module);
    return *this;
  }

  Container &Container::propagate_to(EventManager &em) {
    listen(MIDI_ANY, [&] (const Event &e) {
      em.respond_to(e);
    });
    return *this;
  }

  Container &Container::connect(Module &source, string output, Module &dest, string input) {
      
    Wire wire = {
      source,
      source.get_output_index(output),
      dest,
      dest.get_input_index(input)
    };

    wires.push_back(wire);

    return *this;

  }

  Container &Container::connect(Module &source, string output, Module &dest, int input) {
      
    Wire wire = {
      source,
      source.get_output_index(output),
      dest,
      input
    };

    wires.push_back(wire);

    return *this;

  }  

  Container &Container::disconnect(Module &source, string output, Module &dest, string input) {
    // TODO
    return *this;

  }

  void Container::init() {

    // call all init methods
    for(Module * m : modules) {
      m->init();
    }    

  }

  void Container::update() {

    // WIRES
    for(Wire & w : wires) {
      DEST(w).set_input(INPUT(w), SOURCE(w).get_output(OUTPUT(w)));
    }

    // MODULES
    for(Module * m : modules) {
      m->update();
    }

    // EVENTS
    process_events(modules);

  }

  void Container::run(int num_steps) {

    signal(SIGINT, sighandler);

    init();

    // loop
    for(int i=0; !interuppted && ( num_steps < 0 || i<num_steps ); i++ ) {
      update();
    }
    
  } 

}
