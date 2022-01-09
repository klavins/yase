#ifndef YASE_MIDI_H
#define YASE_MIDI_H

#include <vector>
#include <string>
#include "module.hh"
#include "../../rtmidi/RtMidi.h"

namespace yase {

    class Midi : public Module {

    public:

      Midi();
      void init();
      void update();
      int get_port_id(std::string device_name);

    private:

      vector<RtMidiIn *> midi_inputs;
      vector<string> port_names;
      std::vector<unsigned char> message;

    };

}

#endif