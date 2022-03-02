#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Wavetable wt("./wavetables/triangle-square.wav");
    Sine lfo;
    Transform adjust([&] (double u) { return (1+u)/2; });
    Envelope envelope;
    Audio audio;
    Container synth;
    Timer timer;

    wt.set_input("frequency", 220);
    lfo.set_input("frequency", 2);

    double dt = 0.75;

    envelope.set_input("attack", dt);
    envelope.set_input("decay", dt);
    envelope.set_input("sustain", 1);
    envelope.set_input("release", dt);
    envelope.set_input("velocity", 1);

    synth.add(wt)
         .add(lfo)
         .add(adjust)
         .add(audio)
         .add(envelope)
         .add(timer);

    synth.connect(lfo, "signal", adjust, "signal")
         .connect(adjust, "signal", wt, "morph")
         .connect(wt, "signal", envelope, "signal")
         .connect(envelope, "signal",audio, "left")
         .connect(envelope, "signal",audio, "right");

    timer.set(0.25, [&] () { 
        envelope.trigger();
        timer.set(3, [&] () { 
            envelope.release();
        });        
    });

    synth.run(4*SAMPLE_RATE);

    return 0; 

}
