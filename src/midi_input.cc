// 
// YASE Midi Input Module Implementation
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

#include <iostream>
#include "midi_input.hh"
#include "yase.hh"

namespace yase {

  int num_ports = 0;

  //! Construct a new MidiInput object. When constructed, the object will attempt to 
  //! find the Midi Device connected to your computer with the given name. 
  //! If it cannot, it will throw an exception. 
  //! \param device_name The name of the device 
  MidiInput::MidiInput(string device_name) : device_name(device_name) {

      std::cout << "\nLooking for " << device_name << " ... ";

      RtMidiIn * temp = new RtMidiIn();
      unsigned int nPorts = temp->getPortCount();
      midi_input = NULL;

      for ( unsigned int i=0; i<nPorts; i++ ) {
          RtMidiIn * mi = new RtMidiIn();
          mi->openPort(i);
          if ( mi->getPortName(i) == device_name ) {
            midi_input = mi;
            _port = i;
            std::cout << "found it!\n";
          } else {
            mi->closePort();
          }
      }

      if ( !midi_input ) {
         throw Exception(std::string("Unkown MIDI input device: ") + device_name);
      }
                  
      delete temp;

  }

  void MidiInput::init() {}

  void MidiInput::update() {

    midi_input->getMessage(&message);

    while ( message.size() != 0 ) { // TODO: Check whether this is a good idea 
                                    // (get data on number of times this loop 
                                    // executes per update)

      if ( message.size() == 3 ) {
          emit(Event(message[0], message[1], message[2], _port)); 
      } else if ( message.size() == 2 ) {
          emit(Event(message[0], message[1], 0, _port)); 
      }

      midi_input->getMessage(&message);

    }

  }    

}
