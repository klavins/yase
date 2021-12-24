#include <iostream>
#include "event.hh"
#include "sequencer.hh"

namespace yase {

  Sequencer::Sequencer() {

  }

  void Sequencer::init() {
    update_fcn = &Sequencer::idle;
    step = 0;
    t = 0;
    period = 0.25; // quarter of a second
    duration = 0.25; // fraction of period
  }

  void Sequencer::update() {
    CALL_MEMBER_FN(this, update_fcn);
  }    

  void Sequencer::recording() {
    // Not much do to here for a key at a time sequencer.
    // This method could keep track of time for quantized
    // sequence entry.
  }

  void Sequencer::playing() {

    t += TS;

    if ( mode == UP && t > period ) {
      mode = DOWN;
      t = 0;
      sequence[step]->code = MIDI_KEYDOWN;
      emit(*sequence[step]);
    } 
    
    if ( mode == DOWN && t > duration * period ) {
      mode = UP;
      sequence[step]->code = MIDI_KEYUP;
      emit(*sequence[step]);
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
      for ( int i=0; i< sequence.size(); i++ ) {
        std::cout << sequence[i]->id << " ";
      }
      std::cout << "\n";
    }
  }

  void Sequencer::keyup(const Event &e) {
    // Do nothing for now, maybe record duration later
    // Or maybe record fader events while pressed
  }

  void Sequencer::reset() {
    std::cout << "resetting\n";
    step = 0;
  }

  void Sequencer::record() {
    std::cout << "recording\n";
    update_fcn = &Sequencer::recording;
  }

  void Sequencer::stop() {
    std::cout << "stopping\n";
    update_fcn = &Sequencer::idle;
  }

  void Sequencer::play() {
    if ( sequence.size() > 0 ) {
      std::cout << "playing\n";
      t = 0; 
      mode = UP;
      update_fcn = &Sequencer::playing;
    }
  }

  void Sequencer::clear() {
    if ( update_fcn == &Sequencer::idle ) { 
      std::cout << "clearing\n";
      for (auto e : sequence) {
        delete e;
      }       
      sequence.clear();
    }
  }

}

 