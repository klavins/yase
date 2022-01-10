#ifndef YASE_MIDI_INPUT_H
#define YASE_MIDI_INPUT_H

#include <vector>
#include <string>
#include "module.hh"
#include "../../rtmidi/RtMidi.h"

namespace yase {

    class MidiInput : public Module {

    public:

      MidiInput(string device_name);
      void init();
      void update();
      inline int port() { return _port; }

    private:

      RtMidiIn * midi_input;
      string device_name;
      std::vector<unsigned char> message;
      int _port;

    };

}

#endif