#include "yase.hh"

namespace yase {

  //! Constructor for a new Buttons object. 
  //! \param device_name The name of the MIDI device who's buttons should be be used.
  Buttons::Buttons(string device_name) {

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

  void Buttons::init() {

  }

  int c = 0;

  void Buttons::update() {
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

  //! Set a button's state.
  //! \param id The MIDI id of the button
  //! \param state The desired state of the button
  void Buttons::set(int id, BUTTON_STATE state) {
    button_states[id] = state;
  }

  //! Turn a button on, illuminating it's LED
  //! \param id The MIDI id of the button
  Buttons &Buttons::on(unsigned char id) {
    std::vector<unsigned char> msg = { 144, id, 1 };
    send(msg);
    button_states[id].on = true;
    return *this;
  }

  //! Turn a button on, illuminating it's LED
  //! \param id The MIDI id of the button
  Buttons &Buttons::on(unsigned char id, unsigned char color) {
    std::vector<unsigned char> msg = { 144, id, color };
    send(msg);
    button_states[id].on = true;
    return *this;
  }  

  Buttons &Buttons::send(std::vector<unsigned char> msg) {
    midi_output->sendMessage(&msg);
    return *this;
  }

  //! Turn a button off, unilluminating it's LED
  //! \param id The MIDI id of the button
  Buttons &Buttons::off(unsigned char id) {
    std::vector<unsigned char> msg = { 144, id, 0 };
    send(msg);
    button_states[id].on = false;
    return *this;
  }

  //! Set a button blinking.
  //! \param id The MIDI id of the button
  //! \param period The period in seconds
  Buttons &Buttons::blink_on(int id, double period) {
    set(id, {false, true, 0, period});
    return *this;
  }

  //! Stop a button blinking
  //! \param id The MIDI id of the button
  Buttons &Buttons::blink_off(int id) {
    set(id, {false, false, 0, 0});
    off(id);
    return *this;
  }

  //! Associate a button with an method to run when the button is pressed.
  //! The LED of the button is illumiated momentarily.
  //! For example
  //! \code{.cpp}
  //! buttons.momentary(123, [](const Event &e) {
  //!   cout << "You pressed buton 123\n";
  //! });
  //! \endcode
  //! \param id The MIDI id of the button
  //! \param handler The method to run
  //!
  Buttons &Buttons::momentary(int id, function<void(const Event &)> handler, int type) {

    set(id, {false, false, 0, 0});

    if ( type == MIDI_KEYDOWN ) {

      listeners[MIDI_KEYDOWN].push_back([this, id, handler] (const Event e) {
        if ( e.port == port_id && e.id == id && e.value > 0 ) {
          handler(e);
          on(e.id);
        } else if ( e.port == port_id && e.id == id && e.value == 0 ) {
          off(e.id);
        }
      });

      listeners[MIDI_KEYUP].push_back([this, id, handler] (const Event e) {
        if ( e.port == port_id && e.id == id ) {
          off(e.id);
        }
      });

    } else if ( type == MIDI_MOD ) {

      listeners[MIDI_MOD].push_back([this, id, handler] (const Event e) {
        if ( e.port == port_id && e.id == id && e.value > 0 ) {
          handler(e);
          on(e.id);
        } else if ( e.port == port_id && e.id == id && e.value == 0 ) {
          off(e.id);
        }
      });

    }

    return *this;

  }

  //! Associate a set of buttons with a set of method to run when the associated 
  //! button is pressed. The LED of the most recently pressed button is
  //! For example
  //! \code{.cpp}
  //! buttons.mutex({123, 234}, [](const Event &e) {
  //!   cout << "You pressed buton 123\n";
  //! }, [](const Event &e) {
  //!   cout << "You pressed buton 234\n";
  //! });
  //! \endcode
  //! \param id The MIDI id of the button
  //! \param handler The method to run
  //!
  Buttons &Buttons::mutex(vector<int> ids, vector<function<void(const Event &)>> handlers) {

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

  //! Set up a toggle button. When it is pressed, the method is run and the led is illumated
  //! (unless init_on is true). When it is pressed again, the LED is turned off. 
  //! \param id The midi id of the button
  //! \param handler The function to call when the button is pressed
  //! \param init_on Whether the led for the button is initially on or off
  Buttons &Buttons::toggle(int id, function<void(const Event &)> handler, bool init_on) {
    set(id, {init_on, false, 0, 0});
    if ( init_on ) {
      on(id);
    }
    listeners[MIDI_ANY].push_back([this, id, handler] (const Event e) {
      if ( ( e.code == MIDI_KEYUP || ( e.code == MIDI_KEYDOWN && e.value == 0 ) ) && e.port == port_id && e.id == id ) {
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

  map<int,BUTTON_STATE> Buttons::get_states() {
    return button_states;
  }

  void Buttons::set_states(map<int,BUTTON_STATE> states) {
    button_states = states;
  }

  //! Turn off all the currently illuminated LEDs
  void Buttons::clear_leds() {
    for ( auto [id, state] : button_states ) {
      off(id);
    }
  }

}
