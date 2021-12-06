#ifndef YASE_MIDI_H
#define YASE_MIDI_H

#include <vector>
#include "yase.hh"
#include "../../rtmidi/RtMidi.h"

namespace yase {

    class Midi : public Module {

    public:

      Midi();
      void init();
      void update();

    private:

      RtMidiIn * midiin;
      std::vector<unsigned char> message;

    };

}

#endif
