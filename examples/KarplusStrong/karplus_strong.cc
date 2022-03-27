#include <iostream>
#include "yase.hh"
#include <random>

using namespace yase;

#define LENGTH (((int) (SAMPLE_RATE/220)) - 1)

class String : public Container {

public:
  
  String() : delay(LENGTH), filter(1, 2) {

    signal = add_output("signal"); 

    add(delay);
    add(filter);

    filter.set_coefficients({2.01},{1,1});

    connect(delay, "signal", filter, "signal");
    connect(filter, "signal", delay, "signal");
    equate_output("signal", delay, "signal");

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0,1.0);

    for(int i=0; i<LENGTH; i++) {
        double y = distribution(generator);
        delay.set_input("signal", y);
        delay.update();
    }

  }

private:

  int signal;
  Delay delay;
  IIRFilter filter;

};

int main(int argc, char * argv[]) {

    String string;
    Audio audio;
    Envelope env;    
    Container synth;

    env.set_input("attack", 0.25);
    env.set_input("decay", 0.5);
    env.set_input("sustain", 1.0);

    synth.add(string)
         .add(audio)
         .add(env)
         .connect(string, "signal", env, "signal")
         .connect(env,"signal",audio,"left")
         .connect(env,"signal",audio,"right");

    env.trigger();
    synth.run(4*SAMPLE_RATE);
    return 0; 

}
 