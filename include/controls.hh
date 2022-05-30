#ifndef YASE_CONTROLS_H
#define YASE_CONTROLS_H

#include "yase.hh"

namespace yase {

    class Controls : public Container {

    public:

      Controls();
      ~Controls();
      void init();
      void update();

      Controls &control(Module &fader, int midi_id);
      Controls &control(Module &module, string name, double min, double max, int midi_id);
      Controls &control(Module &module, int index, double min, double max, int midi_id);

      Controls &set_tracking_gain(double x);
      Controls &linear();
      Controls &exponential(double base);
      
      void randomize_faders(); 

    private:

      std::vector<Fader *> faders;           

    };

}

#endif
