#ifndef YASE_BUTTONS_H
#define YASE_BUTTONS_H

#include <vector>
#include <string>
#include "event_manager.hh"
#include "../../rtmidi/RtMidi.h"

using std::vector;
using std::string;

namespace yase {

    typedef struct {
       bool on, blink;
       double timer;
       double period;
    } BUTTON_STATE;

    class Buttons : public EventManager, public Module {

    public:

      Buttons(string output_device_name);

      void init();
      void update();

      Buttons &on(unsigned char id);
      Buttons &off(unsigned char id);
      Buttons &blink_on(int id, double period);
      Buttons &blink_off(int id);

      Buttons &momentary(int id, function<void(const Event &)> handler);
      Buttons &mutex(vector<int> ids, vector<function<void(const Event &)>> handlers);
      Buttons &toggle(int id, function<void(const Event &)> handler, bool init_on);

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
