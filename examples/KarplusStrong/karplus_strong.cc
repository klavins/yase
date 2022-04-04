#include "yase.hh"

using namespace yase;

class Impulse : public Module {

  public:

  Impulse() : length(1) {
    signal = add_output("signal");
  }

  void init() {}

  void update() {
    if ( n++ < length ) {
      outputs[signal] = 1;
    } else {
      outputs[signal] = 0;
    }
  }

  void trigger() {
    n = 0;
  }

  void set(int k) {
    length = k;
  }

private:

  int length, n, signal;

};

class String : public Container {

public:
  
  String() : filter({2.01}, {1, 1}) {

    frequency = add_input("frequency");
    add_output("signal"); 

    path(sum, delay, filter);
    connect(filter, "signal", sum, 0);
    connect(impulse, "signal", sum, 1);
    equate_output("signal", delay, "signal");

  }

  double length() {
    return ((int) (SAMPLE_RATE/inputs[frequency])) - 3;
  }

  void pluck() {
    delay.clear();
    delay.set(length());
    impulse.set(length()/2);
    impulse.trigger();
  }

private:

  Delay delay;
  IIRFilter filter;
  Impulse impulse;
  Sum sum;
  int frequency;

};

int main(int argc, char * argv[]) {

    String string;
    Audio audio;
    Cycle cycle;    
    Container synth;

    cycle.set({ 440, 587.33, 220, 659.26, 246.94, 293.67 }, [&] (double freq) {
      string.set_input("frequency", freq);
      string.pluck();
    }, 1.0);    

    synth.connect(string,"signal",audio,"left")
         .connect(string,"signal",audio,"right")
         .add(cycle);

    synth.run(18*SAMPLE_RATE);
    return 0; 

}
 