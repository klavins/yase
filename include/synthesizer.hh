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
#include "midi.hh"
#include "button_manager.hh"

namespace yase {

    using std::string;
    using std::vector;
    using std::tuple;
    using std::function;
    using std::map;

    typedef tuple<Module &, int, Module &, int> Wire;

    class Synthesizer : public ButtonManager {

    public:

      Synthesizer(string button_device_name);
      ~Synthesizer();
      void init();
      void update();
      Synthesizer &add(Module &module);
      void run(int num_steps);

      // Connectivity
      Synthesizer &connect(Module &source, string output, Module &dest, string input);
      Synthesizer &connect(Module &source, string output, Module &dest, int input);
      Synthesizer &disconnect(Module &source, string output, Module &dest, string input);

      // Controls
      Synthesizer &control(Module &fader, int midi_id);
      Synthesizer &control(Module &module, string name, double min, double max, int midi_id);
      Synthesizer &control(Module &module, int index, double min, double max, int midi_id);
      void randomize_faders();          

    private:

      vector<Module *> modules;
      vector<Wire> wires;
      int sample_rate; 
      std::vector<Fader *> faders;

    };
}

#endif
