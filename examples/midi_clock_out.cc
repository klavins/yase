#include <iostream>
#include "yase.hh"

using namespace yase;

class Clock : public Module {

    public:

    Clock(string device_name) : counter(0) {
      // Check inputs
      RtMidiIn * temp = new RtMidiIn();
      unsigned int nPorts = temp->getPortCount();

      midi_output = NULL;
      for ( unsigned int i=0; i<nPorts; i++ ) {
          RtMidiOut * mo = new RtMidiOut();
          mo->openPort(i);
          std::cout << mo->getPortName(i) << "\n";
          if ( mo->getPortName(i) == device_name ) {
            midi_output = mo;
            port_id = i;
            std::cout << "Found output device " << device_name << "\n";
          }
      }

      if ( ! midi_output ) {
         throw Exception(std::string("Unkown MIDI output device: ") + device_name);
      }
      
      delete temp;

    }

    void init() {

    }

    void update() {
        if ( counter++ >= 0.5 * SAMPLE_RATE / 24  ) {
            counter = 0;
            std::vector<unsigned char> msg = { 248 };
            midi_output->sendMessage(&msg);            
        }
    }    

    private:

      int counter;
      RtMidiOut * midi_output;
      int port_id;      

};

int main(int argc, char * argv[]) {

    Clock clock("Scarlett 4i4 USB");
    Audio audio;
    Container synth;
    
    synth.add(clock).add(audio);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}