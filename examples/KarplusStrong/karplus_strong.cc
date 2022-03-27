#include <iostream>
#include "yase.hh"
#include <random>

using namespace yase;

#define LENGTH (((int) (SAMPLE_RATE/220)) - 1)

class RandomImpulse : public Module {

  public:

  RandomImpulse(int length) : length(length), n(0), distribution(0,1) {
    signal = add_output("signal");
  }

  void init() {}

  void update() {
    if ( n++ < length ) {
      outputs[signal] = distribution(generator);
    } else {
      outputs[signal] = 0;
    }
  }

private:

  int length, n, signal;
  std::default_random_engine generator;
  std::normal_distribution<double> distribution;

};

class String : public Container {

public:
  
  String() : delay(LENGTH), impulse(LENGTH), filter({2.01}, {1, 1}), sum(2) {

    signal = add_output("signal"); 
    path(sum, delay, filter);
    connect(filter, "signal", sum, 0);
    connect(impulse, "signal", sum, 1);
    equate_output("signal", delay, "signal");

  }

private:

  int signal;
  Delay delay;
  IIRFilter filter;
  RandomImpulse impulse;
  Sum sum;

};

int main(int argc, char * argv[]) {

    String string;
    Audio audio;
    Envelope env;    
    Container synth;

    env.set_input("attack", 0.25);
    env.set_input("decay", 0.5);
    env.set_input("sustain", 1.0);

    synth.connect(string, env)
         .connect(env,"signal",audio,"left")
         .connect(env,"signal",audio,"right");

    env.trigger();
    synth.run(4*SAMPLE_RATE);
    return 0; 

}
 