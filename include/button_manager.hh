#ifndef YASE_BUTTONMANAGER_H
#define YASE_BUTTONMANAGER_H

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

    class ButtonManager : public EventManager, public Module {

    public:

      ButtonManager(string output_device_name);

      void init();
      void update();

      ButtonManager &on(unsigned char id);
      ButtonManager &off(unsigned char id);
      ButtonManager &blink_on(int id, double period);
      ButtonManager &blink_off(int id);

      ButtonManager &momentary(int id, function<void(const Event &)> handler);
      ButtonManager &mutex(vector<int> ids, vector<function<void(const Event &)>> handlers);
      ButtonManager &toggle(int id, function<void(const Event &)> handler, bool init_on);

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
