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

  //! Adds a module to the container. When the container is run, the module will
  //! be updated on each iteration. 
  //! \param module The module to be added
  //! \return A reference to the container for method chaining
  Container &Container::add(Module &module) {
    modules.push_back(&module);
    return *this;
  }

  //! Tell the container to propagate all events to another event manager. Thus, if some
  //! module contained within the container emits an event, the event manager em will
  //! hear the event too. Usuall used in conjunction with add().
  //! \param em The event manager (another module) that should receive events
  //! \return A reference to the container for method chaining
  Container &Container::propagate_to(EventManager &em) {
    listen(MIDI_ANY, [&] (const Event &e) {
      em.respond_to(e);
    });
    return *this;
  }

  //! Connect the source output to the destination input with a virtual wire. Upon each
  //! update, the container will first copy the output of source to the input of
  //! destination, and then it will update the modules. 
  //! \param source The source module
  //! \param output The name of the output of the source module
  //! \param dest The desintation module
  //! \param input The name of the input of the desintation module
  //! \return A reference to the container for method chaining  
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

  //! Connect the source output to the destination input with a virtual wire. Upon each
  //! update, the container will first copy the output of source to the input of
  //! destination, and then it will update the modules. 
  //! \param source The source module
  //! \param output The name of the output of the source module
  //! \param dest The desintation module
  //! \param input The index of the input of the desintation module
  //! \return A reference to the container for method chaining  
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

  //! Not implemented
  //! \todo Implement this method or delete it.
  Container &Container::disconnect(Module &source, string output, Module &dest, string input) {
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

  //! Run the container for the indicated number of steps. Use -1 or UNTIL_INTERRUPTED
  //! for the number of steps to run until interupted by Ctrl-C. 
  //! \param The number of steps to run the container. 
  void Container::run(int num_steps) {

    signal(SIGINT, sighandler);

    init();

    // loop
    for(int i=0; !interuppted && ( num_steps < 0 || i < num_steps ); i++ ) {
      update();
    }
    
  } 

}
