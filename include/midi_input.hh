// 
// YASE Midi Input Module Header
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

#ifndef YASE_MIDI_INPUT_H
#define YASE_MIDI_INPUT_H

#include <vector>
#include <string>
#include "module.hh"
#include "RtMidi.h"

namespace yase {

    //! A Midi Input manager. 

    //! Add one of these to your container for each 
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
    //!
    //! This class is a YASE wrapper of RTMidi (https://www.music.mcgill.ca/~gary/rtmidi/).
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