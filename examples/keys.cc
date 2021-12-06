#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine;
    Audio audio;
    Midi midi;
    Gain gain;
    Synthesizer synth;

    int num_keys = 0;

    sine.set_input("frequency", 0);
    gain.set_input("amplitude", 0);

    synth.add(sine)
         .add(audio)
         .add(midi)
         .add(gain);

    synth.connect(sine,"signal",gain,"signal_in")
         .connect(gain,"signal_out",audio, "left")
         .connect(gain,"signal_out",audio, "right");

    synth.listen(MIDI_KEYDOWN, [&sine, &gain, &num_keys] (const Event &e) {
            sine.set_input("frequency", e.frequency());
            gain.set_input("amplitude", e.value / 127.0);  
            num_keys++;           
         })
         .listen(MIDI_KEYUP, [&sine, &gain, &num_keys] (const Event &e) {
            num_keys--;
            if ( num_keys == 0 ) {
              gain.set_input("amplitude", 0);             
            }
         });         

    synth.run(-1);

    return 0;

}

