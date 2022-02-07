#ifndef YASE_CONTAINER_H
#define YASE_CONTAINER_H

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <typeinfo>
#include <algorithm>

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
    class Container : public Module, public EventManager {

    public:

      Container();
      void init();
      void update();
      Container &add(Module &module);
      Container &propagate_to(EventManager &em);
      void run(int num_steps);

      // Connectivity
      Container &connect(Module &source, string output, Module &dest, string input);
      Container &connect(Module &source, string output, Module &dest, int input);
      Container &disconnect(Module &source, string output, Module &dest, string input);

      // I/O Identifications
      Container &equate_input(string input, Module &sub_module, string sub_input);
      Container &equate_output(string output, Module &sub_module, string sub_output);

    private:

      vector<Module *> modules;
      vector<Wire> wires;
      vector<Equate> input_equates, output_equates;
      
    };
}

#endif
