#include "midi.hh"
#include "yase.hh"

namespace yase {

  Midi::Midi() {
  }

  void Midi::init() {

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
          std::cout << "Input Port #" << i+1 << ": " << portName << '\n';
      }
      
      delete temp;

      std::cout << "Midi ready\n";

  }

  void Midi::update() {

      for ( auto mi : midi_inputs ) {

        mi->getMessage(&message);

        if ( message.size() == 3 ) {
            emit(Event(message[0], message[1], message[2])); 
        } else if ( message.size() == 2 ) {
            emit(Event(message[0], message[1], 0)); 
        } else {
          for ( int i=0; i<message.size(); i++ ) {
            std::cout << " -- " << i << ": " << (int) message[i] << "\n";
          }
        }

      }

  }    

}

