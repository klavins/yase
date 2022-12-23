// 
// YASE MidiOutput Implementation
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

#ifndef YASE_MIDI_OUTPUT_H
#define YASE_MIDI_OUTPUT_H

#include "yase.hh"

namespace yase {

    MidiOutput::MidiOutput(std::string device_name) {

      mo = NULL;

      RtMidiIn * temp = new RtMidiIn();
      unsigned int nPorts = temp->getPortCount();
      delete temp;

      int port_id;

      for ( unsigned int i=0; i<nPorts; i++ ) {
          RtMidiOut * temp = new RtMidiOut();
          temp->openPort(i);
          if ( temp->getPortName(i) == device_name ) {
              mo = temp;
              port_id = i;
              std::cout << " found it!\n";
          }
      }

      if ( ! mo ) {
          throw Exception(std::string("Unkown MIDI output device: ") + device_name);
      }   

    }

    void MidiOutput::send(uint8_t status, uint8_t data1, uint8_t data2 ) {
        msg = { status, data1, data2 };
        mo->sendMessage(&msg); 
    }

}

#endif