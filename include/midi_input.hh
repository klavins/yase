#ifndef YASE_MIDI_INPUT_H
#define YASE_MIDI_INPUT_H

#include <vector>
#include <string>
#include "module.hh"
#include "../../rtmidi/RtMidi.h"

namespace yase {

    //! A Midi Input manager. Add one of these to your container for each 
    //! Midi device you are using. Any MIDI Event received from that device will be
    //! sent to the containing Module. For example
    //! \code
    //! Container container;
    //! MidiInput keyboard("My keyboard make/model");
    //! 
    //! container.add(keyboard);
    //! 
    //! container.listen(MIDI_KEYDOWN, keyboard.port(), [&] (const Event &e) {
    //!   std::cout << "You pressed the key associated with MIDI id " << e.id() << "\n";
    //! });
    //! \endcode
    class MidiInput : public Module {

    public:

      MidiInput(string device_name);
      void init();
      void update();

      //! Return the MIDI port of the device. This is needed in listeners to distinguish among
      //! MidiInput objects when there are more than one. 
      inline int port() { return _port; }

    private:

      RtMidiIn * midi_input;
      string device_name;
      std::vector<unsigned char> message;
      int _port;

    };

}

#endif