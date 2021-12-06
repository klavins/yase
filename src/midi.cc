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
        int code = message[0];
        switch(code) {
          case MIDI_KEYDOWN:
            emit(new MidiEvent("keydown", code, message[1], message[2]));
            break;
          case MIDI_KEYUP:
            emit(new MidiEvent("keyup", code, message[1], message[2]));
            break;  
          case MIDI_BENDER:
            emit(new MidiEvent("bender", code, message[1], message[2]));
            break;          
          case MIDI_MOD:
            emit(new MidiEvent("mod", code, message[1], message[2]));
            break;                                          
        }
      }
  }    

}

