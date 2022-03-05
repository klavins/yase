#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Wavetable wt("./wavetables/i-e-o-a.wav");
    Saw lfo;
    Transform adjust([&] (double u) { return (1+u)/2; });
    Envelope envelope;
    Audio audio;
    Container synth;
    Timer timer;

    wt.set_input("frequency", 110);
    lfo.set_input("frequency", 0.5);
    lfo.set_type("raw");

    double dt = 0.5;

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
        timer.set(7, [&] () { 
            envelope.release();
        });        
    });

    synth.run(8.5*SAMPLE_RATE);

    return 0; 

}
