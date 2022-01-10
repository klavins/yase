#include <iostream>
#include "midi_input.hh"
#include "yase.hh"

namespace yase {

  int num_ports = 0;

  MidiInput::MidiInput(string device_name) : device_name(device_name) {

      std::cout << "Looking for " << device_name << "\n";

      RtMidiIn * temp = new RtMidiIn();
      unsigned int nPorts = temp->getPortCount();

      for ( unsigned int i=0; i<nPorts; i++ ) {
          RtMidiIn * mi = new RtMidiIn();
          mi->openPort(i);
          if ( mi->getPortName(i) == device_name ) {
            midi_input = mi;
            _port = i;
            std::cout << "Found " << device_name << "\n";
          } else {
            mi->closePort();
          }
      }

      if ( ! midi_input ) {
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

