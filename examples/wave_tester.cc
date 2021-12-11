#include <vector>
#include <iostream>
#include <string>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine;
    DirtyTriangle dirty_triangle;
    DirtySaw dirty_saw;
    VanDerPol vanderpol;

    vector<Oscillator *> oscillators = { &sine, &dirty_saw, &dirty_triangle, &vanderpol };
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
        osc->set_input("frequency", 440);
        Fader * fader = new Fader(0,1);
        faders.push_back(fader);
        fader->set_input("target",0.1);
        osc->set_input("amplitude",0.1);
        std::cout << fader << " ";
        synth.add(*osc)
             .add(*fader)
             .control(*fader, "target", midi_ids[i])
             .connect(*fader, "value", *osc, "amplitude")
             .connect(*osc, "signal", sum, "signal_" + std::to_string(i));
        i++;
    }

    synth.connect(sum, "signal", audio, "left");
    synth.connect(sum, "signal", audio, "right");

    synth.run(FOREVER);

    for ( auto f : faders ) {
        delete f;
    }

    return 0;

}

