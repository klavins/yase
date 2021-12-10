#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    DirtyTriangle sine;
    Audio audio;
    Midi midi;
    Envelope env;
    BiquadLPF filter;
    Fader cutoff(200,16000), res(1,20);
    Synthesizer synth;

    int num_keys = 0;

    sine.set_input("frequency", 440);
    env.set_input("attack", 100); // Rate of attack in % per sample
    env.set_input("decay", 1);
    env.set_input("sustain", 1);
    env.set_input("release", 30);
    env.set_input("velocity", 0);

    filter.set_input("frequency", 1000);
    filter.set_input("resonance", 10);

    synth.add(sine)
         .add(audio)
         .add(midi)
         .add(env)
         .add(filter)
         .add(cutoff)
         .add(res);

    synth.connect(sine,"signal",filter,"signal")
         .connect(filter,"signal", env, "signal_in")
         .connect(env,"signal_out",audio, "left")
         .connect(env,"signal_out",audio, "right")
         .connect(cutoff, "value", filter, "frequency")
         .connect(res, "value", filter, "resonance");

    synth.listen(MIDI_KEYDOWN, [&sine, &env, &num_keys] (const Event &e) {
            sine.set_input("frequency", e.frequency());
            env.set_input("velocity", e.value / 127.0);
            env.trigger();
            num_keys++;           
         })
         .listen(MIDI_KEYUP, [&env, &num_keys] (const Event &e) {
            num_keys--;
            if ( num_keys == 0 ) {
              env.release();             
            }
         })
         .listen(MIDI_MOD, [&cutoff, &res] (Event e) {
            if ( e.id == 19 ) {
                cutoff.set_input("target", e.value);
            }
            if ( e.id == 23 ) {
                res.set_input("target", e.value);
            }
         });

    synth.run(FOREVER);

    return 0;

}
