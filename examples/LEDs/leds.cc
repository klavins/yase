// 
// YASE Example
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
#include <unistd.h>
#include <vector>
#include "yase.hh"

#include "RtMidi.h"

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

