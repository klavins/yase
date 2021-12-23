#include <iostream>
#include <unistd.h>
#include <vector>
#include "yase.hh"

#include "../../rtmidi/RtMidi.h"

using namespace yase;

int main(int argc, char * argv[]) {

    vector<RtMidiOut *> midi_outputs;
    vector<string> port_names;  

    std::cout << "Initializing Midi\n";

    // Check inputs
    RtMidiOut * temp = new RtMidiOut();
    unsigned int nPorts = temp->getPortCount();
    std::cout << nPorts << " MIDI input sources available.\n";
    std::string portName;
    int mm_port = 0;

    for ( int i=0; i<nPorts; i++ ) {
        RtMidiOut * mi = new RtMidiOut();
        mi->openPort(i);
        midi_outputs.push_back(mi);
        portName = mi->getPortName(i);
        port_names.push_back(portName);
        std::cout << i << ": " << portName << '\n';
        if ( portName == "MIDI Mix" ) {
          mm_port = i;
        }
    }
    
    delete temp;
    std::cout << "Midi ready\n";

    std::vector<unsigned char> on = { 144, 1, 1 }, 
                              off = { 144, 1, 0 };

    for(int i=0; i<127; i++ ) {
      midi_outputs[mm_port]->sendMessage(&on);
      usleep(100000);
      midi_outputs[mm_port]->sendMessage(&off);
      usleep(100000);
    }

}

