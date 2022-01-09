#include <iostream>
#include "midi.hh"
#include "yase.hh"

namespace yase {

  Midi::Midi() {

      std::cout << "Initializing Midi\n";

      // Check inputs
      RtMidiIn * temp = new RtMidiIn();
      unsigned int nPorts = temp->getPortCount();
      std::cout << nPorts << " MIDI input sources available.\n";
      std::string portName;

      for ( unsigned int i=0; i<nPorts; i++ ) {
          RtMidiIn * mi = new RtMidiIn();
          mi->openPort(i);
          midi_inputs.push_back(mi);
          portName = mi->getPortName(i);
          port_names.push_back(portName);
          std::cout << i << ": " << portName << '\n';
      }
      
      delete temp;
      std::cout << "Midi ready\n";

  }

  void Midi::init() {}

  int Midi::get_port_id(std::string device_name) {
    for ( int i=0; i<port_names.size(); i++ ) {
      if ( port_names[i] == device_name ) {
        return i;
      }
    }
    throw Exception(std::string("Unkown MIDI device name ") + device_name);
  }

  void Midi::update() {

      int port = 0;
      for ( auto mi : midi_inputs ) {

        mi->getMessage(&message); // TODO: is the old value destroyed?

        if ( message.size() == 3 ) {
            emit(Event(message[0], message[1], message[2], port)); 
        } else if ( message.size() == 2 ) {
            emit(Event(message[0], message[1], 0, port)); 
        } else {
          for ( int i=0; i<message.size(); i++ ) {
            std::cout << " PORT " << port << ": " << i << ": " << (int) message[i] << "\n";
          }
        }

        port++;

      }

  }    

}

