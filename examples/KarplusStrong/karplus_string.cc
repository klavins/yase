#include <iostream>
#include "yase.hh"
#include <stdlib.h>

using namespace yase;

#define LENGTH 600

class String : public Container {

  public:
  
  String() : delay(LENGTH), prev(1), mixer(2) {

    signal = add_output("signal"); 

    add(delay);
    add(prev);
    add(mixer);

    mixer.set_amplitude_input(0, 0.499);
    mixer.set_amplitude_input(1, 0.499);

    equate_output("signal", delay, "signal");

    connect(delay, "signal", mixer, 0);
    connect(delay, "signal", prev, "signal");
    connect(prev, "signal", mixer, 1);
    connect(mixer, "signal", delay, "signal");

    for(int i=0; i<LENGTH; i++) {
        double y = (rand()%1000 - 500)/5000.0;
        delay.set_input("signal", y);
        delay.update();
    }

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
    Controls controls;
    Container synth;

    synth.add(string)
         .add(audio)
         .add(controls)
         .propagate_to(controls)
         .connect(string,"signal",audio,"left")
         .connect(string,"signal",audio,"right");

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}
 