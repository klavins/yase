#include "fader_manager.hh"
#include "yase.hh"

int nf = 0;

namespace yase {

  FaderManager::FaderManager() : Container() {
  }

  FaderManager::~FaderManager() {
    for (auto f : faders) {
       delete f;
     } 
     faders.clear();    
  }

  void FaderManager::init() {
    Container::init();
  }

  void FaderManager::update() {
    Container::update();
  }    

  FaderManager &FaderManager::control(Module &fader, int midi_id) {
    listeners[MIDI_MOD].push_back([&fader, midi_id] (Event e) {
      if ( e.id == midi_id ) {
        fader.set_input(0, e.value);
      }
    });
    return *this;
  }

  FaderManager &FaderManager::control(Module &module, string name, double min, double max, int midi_id) {

    Fader * fader = new Fader(min, max);
    add(*fader);
    connect(*fader, "value", module, name);
    control(*fader, midi_id);
    faders.push_back(fader); 
    return *this;

  }

  FaderManager &FaderManager::control(Module &module, int index, double min, double max, int midi_id) {

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
  FaderManager &FaderManager::set_tracking_gain(double x) {
    if ( faders.size() > 0 ) {
      faders.back()->set_tracking_gain(x);
    }
    return *this;
  }

  void FaderManager::randomize_faders() {

    for ( auto f : faders ) {
      f->randomize();
    }

  }  

}

