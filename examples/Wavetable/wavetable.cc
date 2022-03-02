#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Wavetable wt("./wavetables/triangle-square.wav");
    Envelope envelope;
    Audio audio;
    Container synth;
    Timer timer;

    wt.set_input("frequency", 220);

    double dt = 0.9;

    envelope.set_input("attack", dt);
    envelope.set_input("decay", dt);
    envelope.set_input("sustain", 0);
    envelope.set_input("release", 2*dt);
    envelope.set_input("velocity", dt);

    synth.add(wt)
         .add(audio)
         .add(envelope)
         .add(timer);

    synth.connect(envelope, "signal", wt, "morph")
         .connect(wt, "signal",audio, "left")
         .connect(wt, "signal",audio, "right");

    timer.set(0.25, [&] () { 
        envelope.trigger();
    });

    synth.run(8*SAMPLE_RATE);

    return 0; 

}
