#ifndef YASE_FADERMANAGER_H
#define YASE_FADERMANAGER_H

#include "yase.hh"

namespace yase {

    class FaderManager : public Container {

    public:

      FaderManager();
      ~FaderManager();
      void init();
      void update();

      FaderManager &control(Module &fader, int midi_id);
      FaderManager &control(Module &module, string name, double min, double max, int midi_id);
      FaderManager &control(Module &module, int index, double min, double max, int midi_id);
      void randomize_faders(); 

    private:

      std::vector<Fader *> faders;           

    };

}

#endif
