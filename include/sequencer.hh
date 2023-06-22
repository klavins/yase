// 
// YASE Sequencer Module Header
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

#ifndef YASE_SEQUENCER_H
#define YASE_SEQUENCER_H

#include <vector>
#include "event.hh"
#include "yase.hh"

namespace yase {

    //! A general purpose sequencers. 
    
    //! Sequences can be built in two ways.
    //! 1. You can insert MIDI events or rests using to build up an arbitrary sequence. 
    //! 2. You can allocate a sequence of a given size, and set events for each step.
    //! A sequencer requires a Clock input. 
    //! 
    //! \param[input] clock Should be wired to the "signal" output of a Clock object.
    //! \param[input] duration The fraction of the clock period between 0 and 1 to hold a note down before releasing it.
    class Sequencer : public Module {

    typedef  void (Sequencer::*UpdateFunction)();  
    static const int DOWN = 0;
    static const int UP = 1;

    public:

      Sequencer();
      ~Sequencer();

      void init();
      void update();

      void keydown(const Event &e);
      void keyup(const Event &e);
      void insert_rest();

      void reset();
      void record();
      void stop();
      void play();
      void clear();

      void allocate(int n);
      void set(int index, int note);
      bool is_rest(int index);
      void rest(int index);

      void decrease_duration(double amount);
      void increase_duration(double amount);

      bool is_playing();

    private:

      vector<Event *> sequence;
      int step, mode;
      double t, period, prev_clock;
      int duration, clock;
      bool tick;

      UpdateFunction update_fcn;

      void recording();
      void playing();
      void idle();

    };

}
 
#endif
