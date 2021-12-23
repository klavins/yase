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
#include "fader.hh"

namespace yase {

    using std::string;
    using std::vector;
    using std::tuple;
    using std::function;
    using std::map;

    typedef tuple<Module &, int, Module &, int> Wire;

    class Synthesizer : public Module {

    public:

      Synthesizer();
      ~Synthesizer();
      void init();
      void update();
      Synthesizer &add(Module &module);
      Synthesizer &listen(int event_type, function<void(const Event &)> handler);
      Synthesizer &connect(Module &source, string output, Module &dest, string input);
      Synthesizer &connect(Module &source, string output, Module &dest, int input);
      Synthesizer &disconnect(Module &source, string output, Module &dest, string input);
      Synthesizer &control(Module &fader, int midi_id);
      Synthesizer &control(Module &module, string name, double min, double max, int midi_id);
      Synthesizer &control(Module &module, int index, double min, double max, int midi_id);
      Synthesizer &button(int port, int midi_id, function<void(const Event &)> handler);

      void run(int num_steps);

    private:

      vector<Module *> modules;
      vector<Wire> wires;
      int sample_rate; 

      map<int,vector<function<void(const Event&)>>> listeners;
      vector<Fader *> faders;

    };
}

#endif
