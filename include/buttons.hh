// 
// YASE Buttons Module Header
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

#ifndef YASE_BUTTONS_H
#define YASE_BUTTONS_H

#include <vector>
#include <string>
#include "event_manager.hh"
#include "RtMidi.h"
#include "globals.hh"

using std::vector;
using std::string;

namespace yase {

    //! Possible states for a button managed by a Buttons object.
    typedef struct {
       bool on, blink;
       double timer;
       double period;
    } BUTTON_STATE;

    //! A class for managing MIDI buttons. 
    class Buttons : public EventManager, public Module {

    public:

      Buttons(string output_device_name);

      void init();
      void update();

      Buttons &on(unsigned char id);
      Buttons &on(unsigned char id, unsigned char color);
      Buttons &off(unsigned char id);
      Buttons &blink_on(int id, double period);
      Buttons &blink_off(int id);

      Buttons &momentary(int id, function<void(const Event &)> handler, int type=MIDI_KEYDOWN);
      Buttons &mutex(vector<int> ids, vector<function<void(const Event &)>> handlers);
      Buttons &toggle(int id, function<void(const Event &)> handler, bool init_on);
      
      Buttons &send(std::vector<unsigned char> msg);

      void set(int id, BUTTON_STATE state);
      map<int,BUTTON_STATE> get_states();
      void set_states(map<int,BUTTON_STATE> states);

      void clear_leds();

    private:

      RtMidiOut * midi_output;
      int port_id;
      map<int,BUTTON_STATE> button_states;

    };

}

#endif
