#include <vector>
#include <iostream>
#include <string>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine;
    Saw add;
    Triangle tri;
    Square square;
    VanDerPol vdp;
    Noise noise;

    add.set_type("additive");

    vector<Oscillator *> oscillators = { 
        &sine,
        &add, 
        &tri, 
        &square, 
        &vdp,
        &noise };
        
    vector<Fader *> faders;

    int midi_ids[] = { 19, 23, 27, 31, 49, 53, 57, 61 };

    Summer sum(oscillators.size());
    Midi midi;
    Audio audio;
    Synthesizer synth;

    synth.add(sum)
         .add(audio)
         .add(midi);

    int i = 0;
    for ( auto osc : oscillators ) {
        Fader * fader = new Fader(0,1);
        faders.push_back(fader);
        synth.add(*osc)
             .add(*fader)
             .control(*fader, midi_ids[i])
             .connect(*fader, "value", *osc, "amplitude")
             .connect(*osc, "signal", sum, "signal_" + std::to_string(i));
        fader->set_input("target",0);
        osc->set_input("amplitude",0);
        i++;
    }

    faders[0]->set_input("target", 64);

    synth.connect(sum, "signal", audio, "left");
    synth.connect(sum, "signal", audio, "right");

    synth.run(FOREVER);

    for ( auto f : faders ) {
        delete f;
    }

    return 0;

}

