#include "controls.hh"
#include "yase.hh"

int nf = 0;

namespace yase {

  Controls::Controls() : Container() {
  }

  Controls::~Controls() {
    for (auto f : faders) {
       delete f;
     } 
     faders.clear();    
  }

  void Controls::init() {
    Container::init();
  }

  void Controls::update() {
    Container::update();
  }    

  //! Associate a MIDI listener with a Fader. Typically this method is not called directly by the user.
  //! \param fader A reference to a Fader object
  //! \param midi_it The MIDI id of the controller
  Controls &Controls::control(Module &fader, int midi_id) {
    listeners[MIDI_MOD].push_back([&fader, midi_id] (Event e) {
      if ( e.id == midi_id ) {
        fader.set_input(0, e.value);
      }
    });
    return *this;
  }

  //! Associate a controller with an input of a module.
  //! \param module The target module
  //! \param name The name of the target module's input
  //! \param min The minimum output of the control
  //! \param max The minimum maximum value of the control
  //! \param midi_id The MIDI id of the knob or fader on your MIDI controller
  Controls &Controls::control(Module &module, string name, double min, double max, int midi_id) {

    Fader * fader = new Fader(min, max);
    add(*fader);
    connect(*fader, "value", module, name);
    control(*fader, midi_id);
    faders.push_back(fader); 
    return *this;

  }

  //! Associate a controller with an input of a module.
  //! \param module The target module
  //! \param index The index of the target module's input
  //! \param min The minimum output of the control
  //! \param max The minimum maximum value of the control
  //! \param midi_id The MIDI id of the knob or fader on your MIDI controller
  Controls &Controls::control(Module &module, int index, double min, double max, int midi_id) {

    Fader * fader = new Fader(min, max);
    add(*fader);
    connect(*fader, "value", module, index);
    control(*fader, midi_id);
    faders.push_back(fader);
    return *this;

  }    

  //! Sets the tracking gain of the most recently added control. See
  //! Fader::set_tracking_gain(double x).
  //! \param x The new tracking gain (e.g. 0.1*FADER_GAIN)
  //! \return A reference for method chaining
  Controls &Controls::set_tracking_gain(double x) {
    if ( faders.size() > 0 ) {
      faders.back()->set_tracking_gain(x);
    }
    return *this;
  }

  //! Set the most recently added control's response to linear
  Controls &Controls::linear() {
    if ( faders.size() > 0 ) {
      faders.back()->linear();
    }
    return *this;
  }

  //! Set the most recently added control's response to exponential with the given base
  //! \param base The base of the exponential response. 
  Controls &Controls::exponential(double base) {
    if ( faders.size() > 0 ) {
      faders.back()->exponential(base);
    }
    return *this;
  }

  //! Randomly assign a value to all controls
  void Controls::randomize() {
    for ( auto f : faders ) {
      f->randomize();
    }
  }  

}

