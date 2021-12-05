#ifndef YASE_SYNTHESIZER_H
#define YASE_SYNTHESIZER_H

#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "module.hh"

namespace yase {

    using std::string;
    using std::vector;
    using std::tuple;

    typedef tuple<Module &, int, Module &, int> Wire;

    class Synthesizer {

    public:

      Synthesizer(int rate);
      Synthesizer &add(Module &module);
      Synthesizer &connect(Module &source, string output, Module &dest, string input);
      Synthesizer &disconnect(Module &source, string output, Module &dest, string input);

      void update();
      void run();
      void run(int num_steps);

    private:

      vector<Module *> modules;
      vector<Wire> wires;
      int sample_rate;    

    };

}

#endif
