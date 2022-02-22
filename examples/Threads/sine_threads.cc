#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sines[10];
    Mixer mixer(10);
    Audio audio;
    Container synth;

    synth.set_thread_number(11);

    for ( int i=0; i<10; i++ )  {
      sines[i].set_input("frequency", 440);
      synth.add(sines[i], 0);
      synth.connect(sines[i], "signal", mixer, i);
     }

    synth.add(audio,0)
         .add(mixer,0)
         .connect(mixer,"signal",audio,"left")
         .connect(mixer,"signal",audio,"right");

    synth.run_threaded(2*SAMPLE_RATE);

    return 0; 

}

 