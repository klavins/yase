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
          RtMidiOut * mo = new RtMidiOut();
          mi->openPort(i);
          mo->openPort(i);
          midi_inputs.push_back(mi);
          midi_outputs.push_back(mo);
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
            // std::cout << "port " << port << ": "
            //           << int(message[0]) << ", " 
            //           << int(message[1]) << ", " 
            //           << int(message[3]) << "\n";
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

  Midi &Midi::on(int port, unsigned char led) {
    std::vector<unsigned char> msg = { 144, led, 1 };
    midi_outputs[port]->sendMessage(&msg);
    return *this;
  }

  Midi &Midi::off(int port, unsigned char led) {
    std::vector<unsigned char> msg = { 144, led, 0 };
    midi_outputs[port]->sendMessage(&msg);
    return *this;
  }

  void Midi::shutdown(void) {
    for ( int i=0; i<128; i++ ) {
      for ( int j=0; j<midi_outputs.size(); j++ ) {
        off(j,i);
      }
    }
  }

}

