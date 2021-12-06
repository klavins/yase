#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine;
    Audio audio;
    Midi midi;
    Envelope env;
    Synthesizer synth;

    int num_keys = 0;

    sine.set_input("frequency", 440);
    env.set_input("attack", 5);
    env.set_input("decay", 1);
    env.set_input("sustain", 1);
    env.set_input("release", 10);
    env.set_input("velocity", 0);

    synth.add(sine)
         .add(audio)
         .add(midi)
         .add(env);

    synth.connect(sine,"signal",env,"signal_in")
         .connect(env,"signal_out",audio, "left")
         .connect(env,"signal_out",audio, "right");

    synth.listen(MIDI_KEYDOWN, [&sine, &env, &num_keys] (const Event &e) {
            sine.change_frequency(e.frequency());
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

    synth.run(FOREVER);

    return 0;

}

