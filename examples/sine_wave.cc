#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine1, sine2;
    Audio audio;
    Synthesizer synth(SAMPLE_RATE);

    sine1.set_input("frequency", 440);
    sine2.set_input("frequency", 444);

    synth.add(sine1)
         .add(sine2)
         .add(audio)
         .connect(sine1,"signal",audio,"left")
         .connect(sine2,"signal",audio, "right");

    synth.run(100000);

    return 0;

}

