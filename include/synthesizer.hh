#ifndef YASE_SYNTHESIZER_H
#define YASE_SYNTHESIZER_H

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <typeinfo>
#include <algorithm>

#include "module.hh"
#include "event.hh"

namespace yase {

    using std::string;
    using std::vector;
    using std::tuple;
    using std::function;
    using std::map;

    typedef tuple<Module &, int, Module &, int> Wire;

    class Synthesizer {

    public:

      Synthesizer();
      Synthesizer &add(Module &module);
      Synthesizer &listen(const string &event_type_name, function<void(Event *)> handler);
      Synthesizer &connect(Module &source, string output, Module &dest, string input);
      Synthesizer &disconnect(Module &source, string output, Module &dest, string input);

      void update();
      void run();
      void run(int num_steps);

    private:

      vector<Module *> modules;
      vector<Wire> wires;
      int sample_rate; 

      map<string,vector<function<void(Event *)>>> listeners;

    };

}

#endif
