#include "synthesizer.hh"

#define SOURCE(__wire__) std::get<0>(__wire__)
#define OUTPUT(__wire__) std::get<1>(__wire__)
#define DEST(__wire__)   std::get<2>(__wire__)
#define INPUT(__wire__)  std::get<3>(__wire__)

namespace yase {

  Synthesizer::Synthesizer(int rate) : sample_rate(rate) {

  }

  Synthesizer &Synthesizer::add(Module &module) {
    modules.push_back(&module);
    return *this;
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

  Synthesizer &Synthesizer::disconnect(Module &source, string output, Module &dest, string input) {
    // TODO
    return *this;

  }

  void Synthesizer::update() {
    // TODO
  }

  void Synthesizer::run() {
    // call all init methods
    // loop
  }

  void Synthesizer::run(int num_steps) {
    // call all initi methods
    for(Module * m : modules) {
      m->init();
    }
    // loop
    for(int i =0; i< num_steps; i++) {
      for(Wire & w : wires) {
        DEST(w).set_input(INPUT(w), SOURCE(w).get_output(OUTPUT(w)));
      }
      for(Module * m : modules) {
        m->update();
      }
    }
  } 

}

