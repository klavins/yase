#include <iostream>
#include "event.hh"
#include "sequencer.hh"

#define DURATION inputs[duration]

namespace yase {

  Sequencer::Sequencer() {

    duration = add_input("duration");
    clock = add_input("clock");

    inputs[duration] = 0.5;
    inputs[clock] = -1;
    prev_clock = -1;

  }

  void Sequencer::init() {
    update_fcn = &Sequencer::idle;
    step = 0;
    t = 0;
    prev_clock = inputs[clock];
    mode = UP;
    tick = false;
  }

  void Sequencer::update() {
    CALL_MEMBER_FN(this, update_fcn);
    t += TS;
    tick = (prev_clock < inputs[clock]);
    prev_clock = inputs[clock];    
    if ( tick ) {
      period = t;
      t = 0;
    }
  }

  void Sequencer::recording() {
    // Not much do to here for a key at a time sequencer.
    // This method could keep track of time for quantized
    // sequence entry.
  }

  void Sequencer::playing() {

    if ( mode == UP && tick ) {
      mode = DOWN;
      if ( sequence[step]->code != SEQUENCE_REST ) {
        sequence[step]->code = MIDI_KEYDOWN;
        emit(*sequence[step]);
      }
    } 
    
    if ( mode == DOWN && t > DURATION * period ) {
      mode = UP;
      if ( sequence[step]->code != SEQUENCE_REST ) {
        sequence[step]->code = MIDI_KEYUP;
        emit(*sequence[step]);
      }
      step++;
      if ( step >= sequence.size() ) {
        step = 0;
      }
    }

  }

  void Sequencer::idle() {
     // do nothing
  }

  void Sequencer::keydown(const Event &e) {
    if ( update_fcn == &Sequencer::recording ) {
      sequence.push_back(new Event(e));
    }
  }

  void Sequencer::keyup(const Event &e) {
    // Do nothing for now, maybe record duration later
    // Or maybe record fader events while pressed
  }

  void Sequencer::insert_rest() {
      if ( update_fcn == &Sequencer::recording ) {
        sequence.push_back(new Event(SEQUENCE_REST, 0, 0, 0));
      }
  }

  void Sequencer::reset() {
    step = 0;
  }

  void Sequencer::record() {
    update_fcn = &Sequencer::recording;
  }

  void Sequencer::stop() {
    update_fcn = &Sequencer::idle;
    if ( mode == DOWN ) {
      sequence[step]->code = MIDI_KEYUP;
      emit(*sequence[step]); 
      mode = UP;   
    }
  }

  void Sequencer::play() {
    if ( sequence.size() > 0 ) {
      t = 0; 
      mode = UP;
      update_fcn = &Sequencer::playing;
    }
  }

  void Sequencer::clear() {
    if ( update_fcn == &Sequencer::idle ) { 
      for (auto e : sequence) {
        delete e;
      }       
      sequence.clear();
    }
  }

  void Sequencer::decrease_duration(double amount) {
      double dur = inputs[duration];
      if ( dur > 0.01 + amount ) {
          inputs[duration] = dur - amount;
      }
  }

  void Sequencer::increase_duration(double amount) {
      double dur = inputs[duration];
      if ( dur < 0.99 - amount ) {
          inputs[duration] = dur + amount;
      }
  } 

  void Sequencer::allocate(int n) {
      for ( int i=0; i<n; i++ ) {
        sequence.push_back(new Event(SEQUENCE_REST, 0, 0, 0));
      }    
  }

  void Sequencer::set(int index, int note) {
      //delete sequence[index];
      sequence[index] = new Event(MIDI_KEYDOWN, note, 127, 0);
  }

  bool Sequencer::is_rest(int index) {
      return sequence[index]->code == SEQUENCE_REST;
  }

  void Sequencer::rest(int index) {
      sequence[index] = new Event(SEQUENCE_REST, 0, 0, 0);
  }

}
