#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sample sample(argv[1]);
    Audio audio;
    Container synth;

    synth.add(sample)
         .add(audio)
         .connect(sample,"left",audio,"left")
         .connect(sample,"right",audio,"right");

    sample.trigger();
    synth.run(sample.size());

    return 0; 

}
  
