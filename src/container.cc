// 
// YASE Container Module Implementation
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <chrono>
#include <mutex>
#include <unistd.h>
#include <pthread.h>
#include <list>
#include "yase.hh"

// Convenience macros for wires
#define SOURCE(__wire__) std::get<0>(__wire__)
#define OUTPUT(__wire__) std::get<1>(__wire__)
#define DEST(__wire__)   std::get<2>(__wire__)
#define INPUT(__wire__)  std::get<3>(__wire__)

// Convenience macros for equates
#define MAIN_INPUT(__equate__)  std::get<0>(__equate__)
#define MAIN_OUTPUT(__equate__) std::get<0>(__equate__)
#define SUB_MODULE(__equate__)  std::get<1>(__equate__)
#define SUB_INPUT(__equate__)   std::get<2>(__equate__)
#define SUB_OUTPUT(__equate__)  std::get<2>(__equate__)

bool interuppted = false;

void sighandler(int sig) {
	  interuppted = true;
}

namespace yase {

  Container::Container() {

  }

  //! Adds a module to the container. When the container is run, the module will
  //! be updated on each iteration. Note that if you use Module::connect to connect
  //! two modules, those modules will be automatically added so you don't need to 
  //! add them with this method. 
  //! \param module The module to be added
  //! \return A reference to the container for method chaining
  Container &Container::add(Module &module) {
    modules.push_back(&module);
    return *this;
  }

  //! Add if the module is not already added
  //! \param module The module to be added
  //! \return A reference to the container for method chaining
  Container &Container::add_if_new(Module &module) {
    bool found = false;
    for ( auto m : modules ) {
      if ( m == &module ) {
        found = true;
      }
    }
    if ( !found ) {
      add(module);
    }
    return *this;
  }

  //! Tell the container to propagate all events to another event manager. Thus, if some
  //! module contained within the container emits an event, the event manager em will
  //! hear the event too. Usually used in conjunction with add().
  //! \param em The event manager (another module) that should receive events
  //! \return A reference to the container for method chaining
  //! 
  //! In the following example, a Controls object is constructed, added, and propagated to.
  //! When MidiEvents are received by the container, they are sent to the control, which 
  //! responds to them by adjusting the associated input. 
  //!
  //! \include Faders/faders.cc
  //! 
  Container &Container::propagate_to(EventManager &em) {
    listen(MIDI_ANY, [&] (const Event &e) {
      em.respond_to(e);
    });
    return *this;
  }

  //! Connect the source output to the destination input with a virtual wire. Upon each
  //! update, the container will first copy the output of source to the input of
  //! destination, and then it will update the modules. 
  //! NOTE: This method has the side effect that it adds both source and dest to the container, if they are not alreaddy added  
  //! \param source The source module
  //! \param output The name of the output of the source module
  //! \param dest The desintation module
  //! \param input The name of the input of the desintation module
  //! \return A reference to the container for method chaining  
  Container &Container::connect(Module &source, string output, Module &dest, string input) {

    add_if_new(source);
    add_if_new(dest);
      
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
  //! NOTE: This method has the side effect that it adds both source and dest to the container, if they are not alreaddy added
  //! \param source The source module
  //! \param output The name of the output of the source module
  //! \param dest The desintation module
  //! \param input The index of the input of the desintation module
  //! \return A reference to the container for method chaining  
  Container &Container::connect(Module &source, string output, Module &dest, int input) {

    add_if_new(source);
    add_if_new(dest);    
      
    Wire wire = {
      source,
      source.get_output_index(output),
      dest,
      input
    };

    wires.push_back(wire);

    return *this;

  }

  //! Connect a parameter autoloader to the inputs of the given module. The autoloader
  //! should be used with a *.json file that has an object called "category" in it that 
  //! in turn has keys with the same names as module. The values associated with the keys
  //! the keys should be numbers and will be connected to the modules inputs. Once
  //! configured, the user can edit the *.json file, save the changes, and the inputs
  //! to the module will be changed.
  //! \param loader an AutoLoad object
  //! \param string the name of the category in the *.json file to use
  //! \param module The module to connect
  Container &Container::connect(AutoLoad& loader, string category, Module& module) {

      for (int i=0; i<module.num_inputs(); i++) {
        string input_name = module.get_input_name(i),
               output_name = category + ":" + input_name;
        if ( loader.provides(category, input_name) ) {
          loader.associate(category, input_name, module);
          connect(loader, output_name, module, input_name);
        } else {
          cout << output_name << " not provided by auto-loader. Hope that's ok.\n";
        }
      }
      return *this;
  }

  //! Connect the two modules, assuming the output and input are called "signal"
  //! NOTE: This method has the side effect that it adds both source and dest to the container, if they are not alreaddy added
  //! \param source The source module
  //! \param dest The desintation module
  //! \return A reference to the container for method chaining    
  Container &Container::connect(Module &source, Module &dest) {
      return connect(source, "signal", dest, "signal");
  }

  //! Connect the three modules into a path, assuming the outputs and inputs are called "signal"
  //! NOTE: This method has the side effect that it adds all modules container, if they are not alreaddy added
  //! \param a The first module
  //! \param b The second module
  //! \param c The third module
  //! \return A reference to the container for method chaining    
  Container &Container::path(Module &a, Module &b, Module &c) {
      connect(a, "signal", b, "signal");
      connect(b, "signal", c, "signal");
      return *this;
  }

  //! Connect the four modules into a path, assuming the outputs and inputs are called "signal"
  //! NOTE: This method has the side effect that it adds all modules container, if they are not alreaddy added
  //! \param a The first module
  //! \param b The second module
  //! \param c The third module
  //! \param d The fourth module  
  //! \return A reference to the container for method chaining    
  Container &Container::path(Module &a, Module &b, Module &c, Module &d) {
      connect(a, "signal", b, "signal");
      connect(b, "signal", c, "signal");
      connect(c, "signal", d, "signal");
      return *this;
  }

  //! Connect the four modules into a path, assuming the outputs and inputs are called "signal"
  //! NOTE: This method has the side effect that it adds all modules container, if they are not alreaddy added
  //! \param a The first module
  //! \param b The second module
  //! \param c The third module
  //! \param d The fourth module 
  //! \param e The fifth module 
  //! \return A reference to the container for method chaining    
  Container &Container::path(Module &a, Module &b, Module &c, Module &d, Module &e) {
      connect(a, "signal", b, "signal");
      connect(b, "signal", c, "signal");
      connect(c, "signal", d, "signal");
      connect(d, "signal", e, "signal");
      return *this;
  }  

  //! Equate the main input of this module to the sub-input of the sub-module. Whenever the 
  //! main module's input is updated, the sub-modules input will also be updated.
  //! \param input A string identifying the main input
  //! \param sub_module A reference to the sub-module
  //! \param sub_input A string identifying the sub-module input
  Container &Container::equate_input(string input, Module &sub_module, string sub_input) {

    Equate equate {
      get_input_index(input),                 // MAIN_INPUT
      sub_module,                             // SUB_MODULE
      sub_module.get_input_index(sub_input)   // SUB_INPUT
    };

    input_equates.push_back(equate);

    return *this;

  }


  //! Attach the inputs this module to the inputs of sub-modules. Calls add_input and 
  //! equate_input on each tuple 
  //! \param attachements A vector of tuples of the form {{string,Module&,string}, ...}
  Container &Container::attach_inputs(std::vector<std::tuple<string,Module&,string>> attachments) {

    for(auto [container_input, module, module_input] : attachments) {
      add_input(container_input);
      equate_input(container_input, module, module_input);
    }

    return *this;

  }

  //! Attach outputs of this module to the outputs of sub-modules. Calls add_input and 
  //! equate_input on each tuple 
  //! \param attachements A vector of tuples of the form {{string,Module&,string}, ...}
   Container &Container::attach_outputs(std::vector<std::tuple<string,Module&,string>> attachments) {

    for(auto [container_output, module, module_output] : attachments) {
      add_output(container_output);
      equate_output(container_output, module, module_output);
    }

    return *this;

  }  

  //! Equate the main output of this module to the sub-output of the sub-module. After the 
  //! sub-module has been updated, it's output will by copied to the main module output.
  //! \param output A string identifying the main output
  //! \param sub_module A reference to the sub-module
  //! \param sub_output A string identifying the sub-module output
  Container &Container::equate_output(string output, Module &sub_module, string sub_output) {

    Equate equate {
      get_output_index(output),                  // MAIN_OUTPUT
      sub_module,                               // SUB_MODULE
      sub_module.get_output_index(sub_output)   // SUB_OUTPUT
    };

    output_equates.push_back(equate);

    return *this;

  }

  //! Remove a connection
  //! \param source The source module
  //! \param output The name of the output of the source module
  //! \param dest The desintation module
  //! \param input The name of the input of the desintation module
  Container &Container::disconnect(Module &source, string output, Module &dest, string input) {

    int index = -1;
    int u = source.get_output_index(output);
    int v = dest.get_input_index(input);
    for ( int i=0; i<wires.size(); i++ ) {
        if ( &SOURCE(wires[i]) == &source &&
             OUTPUT(wires[i]) == u &&
             &DEST(wires[i]) == &dest &&
             INPUT(wires[i]) == v ) {
             index = i;
         }
    }
    if ( index >=0 ) {
        wires.erase(wires.begin() + index);
    }
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

    // INPUT EQUATES
    for(Equate & e : input_equates) {
      SUB_MODULE(e).set_input(SUB_INPUT(e), inputs[MAIN_INPUT(e)]);
    }

    // MODULES
    for(Module * m : modules) {
      m->update();
    }

    // OUTPUT EQUATES
    for(Equate & e : output_equates) {
      outputs[MAIN_OUTPUT(e)] = SUB_MODULE(e).get_output(SUB_OUTPUT(e));
    }

    // EVENTS
    process_events(modules);

  }

  //! Run the container for the indicated number of steps. Use -1 or UNTIL_INTERRUPTED
  //! for the number of steps to run until interupted by Ctrl-C. 
  //! \param The number of steps argument to run the container. 
  void Container::run(int num_steps) {

    signal(SIGINT, sighandler);

    init();

    for(int i=0; !interuppted && ( num_steps < 0 || i < num_steps ); i++ ) {
      update();
    }
    
  } 

  //! Continue running assuming the container has already been run for some amount of time. 
  //! Contained Module init() methods will not be called again.
  //! Will run for the indicated number of steps. Use -1 or UNTIL_INTERRUPTED
  //! for the number of steps arrgument to run until interupted by Ctrl-C. 
  //! \param The number of steps to run the container. 
  void Container::run_again(int num_steps)   {
    for(int i=0; !interuppted && ( num_steps < 0 || i < num_steps ); i++ ) {
      update();
    }    
  }

  //////////////////////////////////////////////////////////////////////////////////////////////
  // EXPERIMENTAL THREADED UPDATES (Not entirely working well [yet])
  //

  //! EXPERIMENTAL
  Container &Container::set_thread_number(int n) {
    if ( groups.size() == 0 ) {
      for ( int i=0; i<n; i++ ) {
        groups.push_back({});
      }
    } else {
      throw Exception("Attempted to set thread number more than once.");
    }
    std::cout << "Number of threads: " << groups.size() << "\n";
    return *this;
  }

  //! EXPERIMENTAL
  Container &Container::add(Module &module, int n) {

    if ( n >=0 && n < groups.size() ) {
      modules.push_back(&module);
      groups[n].push_back(&module);
    } else {
      throw Exception("Attempted at add a module to a non-existent thread group (index out of range)");
    }

    return *this;
  }

  //! EXPERIMENTAL
  void Container::thread_loop(int i) {
    bool flag;
    while ( true ) {
    //  mtx.lock();
      flag = working_flags[i];
    //  mtx.unlock();
      if ( flag ) {
          for ( auto m : groups[i] ) {   
              m->update();
          }    
          mtx.lock();
          working_flags[i] = false;        
          mtx.unlock();
      } else {
          //usleep(1);
          std::this_thread::sleep_for (1ns);
          //std::this_thread::yield();
      }
    }
  }

  //! EXPERIMENTAL
  void Container::run_threaded(int num_steps) {

    signal(SIGINT, sighandler);

    working_flags.push_back(false); // for zeroth thread
    sched_param params;
    params.sched_priority = 99;

    for ( int i=1; i<groups.size(); i++ ) {
      working_flags.push_back(false);
      threads.push_back(std::thread(&Container::thread_loop, this, i));
      pthread_setschedparam(threads.back().native_handle(), SCHED_RR, &params);
    }

    init();

    // loop
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; !interuppted && ( num_steps < 0 || i < num_steps ); i++ ) {
      update_threaded();
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()/num_steps << " ns / step\n";
    
  }

  //! EXPERIMENTAL
  void Container::update_threaded() {

    // WIRES
    for(Wire & w : wires) {
      DEST(w).set_input(INPUT(w), SOURCE(w).get_output(OUTPUT(w)));
    }

    // INPUT EQUATES
    for(Equate & e : input_equates) {
      SUB_MODULE(e).set_input(SUB_INPUT(e), inputs[MAIN_INPUT(e)]);
    }

    // MODULES

    // Wake up threads
    for ( int i=1; i<working_flags.size(); i++ ) {
      mtx.lock();
      working_flags[i] = true;
      mtx.unlock();
    } 

    // update zeroth thread
    for ( auto m : groups[0] ) {   
        m->update();
    }    

    // wait for other threads to finish
    bool done = false;
    while ( !done ) {
      done = true;
      for ( int i=1; i<working_flags.size(); i++ ) {
        //mtx.lock();
        done = done && (!working_flags[i]);
        //mtx.unlock();
      }
    }

    // OUTPUT EQUATES
    for(Equate & e : output_equates) {
      outputs[MAIN_OUTPUT(e)] = SUB_MODULE(e).get_output(SUB_OUTPUT(e));
    }

    // EVENTS
    process_events(modules);

  }

}
