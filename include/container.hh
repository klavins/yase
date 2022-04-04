#ifndef YASE_CONTAINER_H
#define YASE_CONTAINER_H

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <typeinfo>
#include <algorithm>
#include <thread>
#include <list>

#include "module.hh"
#include "event.hh"
#include "fader.hh"
#include "buttons.hh"

namespace yase {

    using std::string;
    using std::vector;
    using std::tuple;
    using std::function;
    using std::map;

    typedef tuple<Module &, int, Module &, int> Wire;
    typedef tuple<int, Module &, int> Equate;

    //! A module and event manager that can contain other modules. It can be used
    //! as a top level module for constructing an entire synthesizer, or as a way
    //! to group together a few modules into a commonly used function, as with, for example
    //! a phasor which would contain a delay and a sum. 
    //!
    //! A good example of a container is the Echo Module, which contains a Delay, a
    //! a Gain and a Sum. 
    //!
    //! Additionally, most examples use a container to add sub modules to. For example,
    //! the following code sets up a Container called synth, adds a few modules to it,
    //! identifies a module to propagate events to, and makes a couple of connections.
    //! \include faders.cc
    //! 
    class Container : public Module, public EventManager {

    public:

      Container();
      void init();
      void update();
      Container &add(Module &module);
      Container &add_if_new(Module &module);      
      Container &propagate_to(EventManager &em);
      void run(int num_steps);
      void run_again(int num_steps);

      // Connectivity
      Container &connect(Module &source, string output, Module &dest, string input);
      Container &connect(Module &source, string output, Module &dest, int input);

      Container &connect(Module &source, Module &dest); // assumes signal name is "signal"
      Container &path(Module &a, Module &b, Module &c);
      Container &path(Module &a, Module &b, Module &c, Module &d);

      Container &disconnect(Module &source, string output, Module &dest, string input);

      // I/O Identifications
      Container &equate_input(string input, Module &sub_module, string sub_input);
      Container &equate_output(string output, Module &sub_module, string sub_output);

      // Multithreading interface
      Container &set_thread_number(int n);
      Container &add(Module &module, int n);
      void run_threaded(int num_steps);
      void update_threaded();
      void thread_loop(int i);

    private:

      vector<Module *> modules;
      vector<Wire> wires;
      vector<Equate> input_equates, output_equates;

      // Thread groups
      vector<std::thread> threads;
      vector<vector<Module *> > groups;
      vector<bool> working_flags;
      std::mutex mtx;

    };
}

#endif
