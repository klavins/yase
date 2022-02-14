#ifndef YASE_SEQUENCER_H
#define YASE_SEQUENCER_H

#include <vector>
#include "event.hh"
#include "yase.hh"

namespace yase {

    //! A general purpose sequencers. Sequences can be built in two ways.
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
