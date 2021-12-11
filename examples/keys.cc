#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    DirtySaw osc;
    Audio audio;
    Midi midi;
    Envelope env;
    BiquadLPF filter;
    Fader cutoff(1000,12000), res(0.1,20);
    Synthesizer synth;

    int num_keys = 0;

    osc.set_input("frequency", 440);
    env.set_input("attack", 100); // Rate of attack in % per sample
    env.set_input("decay", 1);
    env.set_input("sustain", 1);
    env.set_input("release", 30);
    env.set_input("velocity", 0);
    filter.set_input("frequency", 1000);
    filter.set_input("resonance", 10);

    synth.add(osc)
         .add(audio)
         .add(midi)
         .add(env)
         .add(filter)
         .add(cutoff)
         .add(res);

    synth.connect(osc,"signal",filter,"signal")
         .connect(filter,"signal", env, "signal_in")
         .connect(env,"signal_out",audio, "left")
         .connect(env,"signal_out",audio, "right")
         .connect(cutoff, "value", filter, "frequency")
         .connect(res, "value", filter, "resonance");

    synth.listen(MIDI_KEYDOWN, [&osc, &env, &num_keys] (const Event &e) {
            osc.set_input("frequency", e.frequency());
            env.set_input("velocity", e.value / 127.0);
            env.trigger();
            num_keys++;           
         })
         .listen(MIDI_KEYUP, [&env, &num_keys] (const Event &e) {
            num_keys--;
            if ( num_keys == 0 ) {
              env.release();             
            }
         });

     synth.control(cutoff, "target", 19)
          .control(res, "target", 23);

    synth.run(FOREVER);

    return 0;

}
