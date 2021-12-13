#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Synthesizer synth;     

    Saw osc;
    Audio audio;
    Midi midi;
    Envelope env;
    BiquadLPF filter;
    Fader cutoff(1000,12000), res(0.1,20), volume(0,1);
    Gain gain;

    osc.set_type("additive");

    synth.add(osc)     .add(audio)     .add(midi)
         .add(env)     .add(filter)    .add(cutoff)
         .add(res)     .add(volume)    .add(gain);

    synth.connect( osc,    "signal", filter, "signal")
         .connect( filter, "signal", env,    "signal")
         .connect( env,    "signal", gain,   "signal")
         .connect( gain,   "signal", audio,  "left")
         .connect( gain,   "signal", audio,  "right")
         .connect( cutoff, "value",  filter, "frequency")
         .connect( res,    "value",  filter, "resonance")
         .connect( volume, "value",  gain,   "amplitude");

    int num_keys = 0;

    synth.listen(MIDI_KEYDOWN, [&osc, &env, &num_keys,&filter] (const Event &e) {
            osc.set_input("frequency", e.frequency());
            filter.set_input("offset", e.frequency());
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

     synth.control(cutoff, 19)
          .control(res, 23)
          .control(volume, 62);

    synth.run(FOREVER);

    return 0;

}
