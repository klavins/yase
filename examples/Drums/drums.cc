#include <iostream>
#include <string>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sample kick("./samples/Trap Kicks/trap-kicks-25.wav"),
           snare("./samples/Trap Snares/trap-snare-17.wav");
    Timer t1, t2;
    Audio audio;
    Mixer mixer(4);
    Container synth;
    
    synth.add(kick)
         .add(snare)
         .add(t1)
         .add(t2)
         .add(mixer)
         .add(audio)
         .connect(kick, "left", mixer, 0)
         .connect(kick, "right", mixer, 1)
         .connect(snare, "left", mixer, 2)
         .connect(snare, "right", mixer, 3)         
         .connect(mixer, "signal", audio, "left")
         .connect(mixer, "signal", audio, "right");

    t1.set(0.45, [&]() {  
        kick.trigger(); 
        t1.reset();
    });

    t2.set(0.3, [&]() {  
        snare.trigger(); 
        t2.reset();
    });    
  

    synth.run(UNTIL_INTERRUPTED); 

    return 0;

}