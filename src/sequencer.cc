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

  //! Respond to a keydown Event by adding the event to the end of a 
  //! sequence. Requires the sequencer to be in "recordning" mode via a
  //! previous call to the Sequencer method record()
  //! \param e A MIDI_KEYDOWN Event, perhaps emitted by a Midi module and routed by a Container listen handler
  void Sequencer::keydown(const Event &e) {
    if ( update_fcn == &Sequencer::recording ) {
      sequence.push_back(new Event(e));
    }
  }

  //! Not used
  //! \param e The keyup event
  void Sequencer::keyup(const Event &e) {
    // Do nothing for now, maybe record duration later
    // Or maybe record fader events while pressed
  }

  //! Insert a rest onto the end of the sequence
  void Sequencer::insert_rest() {
      if ( update_fcn == &Sequencer::recording ) {
        sequence.push_back(new Event(SEQUENCE_REST, 0, 0, 0));
      }
  }

  //! Reset the sequence counter to the beginning of the sequence.
  void Sequencer::reset() {
    step = 0;
  }

  //! Put the sequencer into record mode so it will respond to calls to keydown()
  void Sequencer::record() {
    update_fcn = &Sequencer::recording;
  }

  //! Stop the sequencer playing
  void Sequencer::stop() {
    update_fcn = &Sequencer::idle;
    if ( mode == DOWN ) {
      sequence[step]->code = MIDI_KEYUP;
      emit(*sequence[step]); 
      mode = UP;   
    }
  }

  //! Play the sequencer. It will emit MIDI events that look just like MIDI events
  //! coming from a Midi device. So if you, for example, have a midi keyboard set
  //! up with Container listen() handlers, it will also "see" the sequencer events. 
  //! Note that normally a sequencer needs to be added to a Container to work.
  void Sequencer::play() {
    if ( sequence.size() > 0 ) {
      t = 0; 
      mode = UP;
      update_fcn = &Sequencer::playing;
    }
  }

  //! Clear the sequence, erasing all events.
  void Sequencer::clear() {
    if ( update_fcn == &Sequencer::idle ) { 
      for (auto e : sequence) {
        delete e;
      }       
      sequence.clear();
    }
  }

  //! Increase the duration, as a fraction of the period, of time between a
  //! MIDI_KEYDOWN and a MIDI_KEYUP event. Does bounds checking. Note that 
  //! the default duration is 0.5.
  //! \param amount The amount.
  void Sequencer::decrease_duration(double amount) {
      double dur = inputs[duration];
      if ( dur > 0.01 + amount ) {
          inputs[duration] = dur - amount;
      }
  }

  //! Decrease the duration, as a fraction of the period, of time between a
  //! MIDI_KEYDOWN and a MIDI_KEYUP event. Does bounds checking. Note that 
  //! the default duration is 0.5.
  //! \param amount The amount.
  void Sequencer::increase_duration(double amount) {
      double dur = inputs[duration];
      if ( dur < 0.99 - amount ) {
          inputs[duration] = dur + amount;
      }
  } 

  //! Allocate a sequence of n steps, and initialize all of them to be rests.
  //! \param n The length of the sequence
  void Sequencer::allocate(int n) {
      for ( int i=0; i<n; i++ ) {
        sequence.push_back(new Event(SEQUENCE_REST, 0, 0, 0));
      }    
  }

  //! Set the a sequence event at the given index to play the given note
  //! \param index The index of the event
  //! \param note The MIDI id ot eh note to play
  void Sequencer::set(int index, int note) {
      //delete sequence[index];
      sequence[index] = new Event(MIDI_KEYDOWN, note, 127, 0);
  }

  //! Return true of the sequence at the given index is a rest
  //! \param index The given index
  bool Sequencer::is_rest(int index) {
      return sequence[index]->code == SEQUENCE_REST;
  }

  //! Set the sequence at the given index to a rest
  //! \param index The given index
  void Sequencer::rest(int index) {
      sequence[index] = new Event(SEQUENCE_REST, 0, 0, 0);
  }

}
