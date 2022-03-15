#include <iostream>
#include "yase.hh"
#include <random>

using namespace yase;

#define LENGTH (((int) (SAMPLE_RATE/220)) - 3)

class String : public Container {

public:
  
  String() : delay(LENGTH), prev(1), mixer(2) {

    signal = add_output("signal"); 

    add(delay);
    add(prev);
    add(mixer);

    mixer.set_amplitude_input(0, 0.495);
    mixer.set_amplitude_input(1, 0.495);

    equate_output("signal", delay, "signal");

    connect(delay, "signal", mixer, 0);
    connect(delay, "signal", prev, "signal");
    connect(prev, "signal", mixer, 1);
    connect(mixer, "signal", delay, "signal");

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0,1.0);

    for(int i=0; i<LENGTH; i++) {
        double y = distribution(generator);
        delay.set_input("signal", y);
        delay.update();
    }

    // for(int i=0; i<LENGTH; i++) {
    //     double y = (i>=10 && i <=20 ? 1: 0);
    //     delay.set_input("signal", y);
    //     delay.update();
    // }    

  }

  void init() {
    Container::init();
  }

  void update() {
    Container::update();
  }

private:

  int signal;
  Delay delay, prev;
  Mixer mixer;

};

int main(int argc, char * argv[]) {

    String string;
    Audio audio;
    Envelope env;    
    Container synth;

    env.set_input("attack", 0.375);
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
 