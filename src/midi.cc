#include "midi.hh"
#include "yase.hh"

namespace yase {

  Midi::Midi() {
  }

  void Midi::init() {
      std::cout << "Initializing Midi\n";
      midiin = new RtMidiIn();
      midiin->openPort(0);
      std::cout << "Midi ready\n";
  }

  void Midi::update() {
      midiin->getMessage(&message);
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

