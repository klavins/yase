#ifndef YASE_SEQUENCER_H
#define YASE_SEQUENCER_H

#include <vector>
#include "event.hh"
#include "yase.hh"

namespace yase {

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
