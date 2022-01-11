#include "yase.hh"

namespace yase {

  ButtonManager::ButtonManager(string device_name) {

      // Check inputs
      RtMidiIn * temp = new RtMidiIn();
      unsigned int nPorts = temp->getPortCount();

      midi_output = NULL;
      for ( unsigned int i=0; i<nPorts; i++ ) {
          RtMidiOut * mo = new RtMidiOut();
          mo->openPort(i);
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

  void ButtonManager::init() {

  }

  int c = 0;

  void ButtonManager::update() {
    for ( auto &[id, state] : button_states ) {
      if ( state.blink == true ) {
        state.timer += TS;
        if ( state.timer >= state.period ) {
          state.timer = 0;
        }
        if ( !state.on && state.timer < state.period/2 ) {
          on(id);
        } else if ( state.on && state.timer >= state.period/2 ) {
          off(id);
        }
      }
    }
  }

  void ButtonManager::set(int id, BUTTON_STATE state) {
    button_states[id] = state;
  }

  ButtonManager &ButtonManager::on(unsigned char id) {
    std::vector<unsigned char> msg = { 144, id, 1 };
    midi_output->sendMessage(&msg);
    button_states[id].on = true;
    return *this;
  }

  ButtonManager &ButtonManager::off(unsigned char id) {
    std::vector<unsigned char> msg = { 144, id, 0 };
    midi_output->sendMessage(&msg);
    button_states[id].on = false;
    return *this;
  }

  ButtonManager &ButtonManager::blink_on(int id, double period) {
    set(id, {false, true, 0, period});
    return *this;
  }

  ButtonManager &ButtonManager::blink_off(int id) {
    set(id, {false, false, 0, 0});
    off(id);
    return *this;
  }

  ButtonManager &ButtonManager::momentary(int id, function<void(const Event &)> handler) {

    set(id, {false, false, 0, 0});

    listeners[MIDI_KEYDOWN].push_back([this, id, handler] (const Event e) {
      if ( e.port == port_id && e.id == id ) {
        handler(e);
        on(e.id);
      }
    });

    listeners[MIDI_KEYUP].push_back([this, id, handler] (const Event e) {
      if ( e.port == port_id && e.id == id ) {
        off(e.id);
      }
    });

    return *this;

  }

  ButtonManager &ButtonManager::mutex(vector<int> ids, vector<function<void(const Event &)>> handlers) {

      for ( int i=0; i<ids.size(); i++ ) {
        set(ids[i], {i==0, false, 0, 0});
      }

      on(ids[0]);

      listeners[MIDI_KEYDOWN].push_back([this, ids, handlers] (Event e) {

        bool found = (std::find (ids.begin(), ids.end(), e.id) != ids.end());

        if ( e.port == port_id && found ) {
          for ( int i=0; i<ids.size(); i++ ) {
            if ( ids[i] == e.id ) {
              handlers[i](e);
              on(ids[i]);
            } else {
              off(ids[i]);
            }
          }
        }
      });

      return *this;

  }

  //! Set up a toggle button
  //! \param id The midi id of the button
  //! \param id The function to call when the button is pressed
  //! \param init_on Whether the led for the button is initially on or off
  ButtonManager &ButtonManager::toggle(int id, function<void(const Event &)> handler, bool init_on) {
    set(id, {init_on, false, 0, 0});
    if ( init_on ) {
      on(id);
    }
    listeners[MIDI_KEYUP].push_back([this, id, handler] (const Event e) {
      if ( e.port == port_id && e.id == id ) {
        if ( button_states[id].on ) {
          off(id);
        } else {
          on(id);
        }
        handler(e);
      }
    });    
    return *this;
  }

  map<int,BUTTON_STATE> ButtonManager::get_states() {
    return button_states;
  }

  void ButtonManager::set_states(map<int,BUTTON_STATE> states) {
    button_states = states;
  }

  void ButtonManager::clear_leds() {
    for ( auto [id, state] : button_states ) {
      off(id);
    }
  }

}
