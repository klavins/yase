#include <iostream>
#include "yase.hh"
#include "../potato_audio.hh"
#include "io.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Saw raw_osc("raw");
    AntiAlias osc(raw_osc);
    Gain gain;
    PotatoAudio audio;
    IO io;
    Envelope env;
    Biquad filter;
    Transform freq_scale( [] (double u) { return (1-u) * 100 + u * 10000; });
    Transform res_scale ( [] (double u) { return (1-u) * 1 + u * 20; });
    Transform invert ( [] (double u) { return -u; });
    Player player;
    Timer timer;
    Container synth;

    osc.set_input("frequency", 55);
    gain.set_input("amplitude", 0.25);

    synth.connect(io, "knob_0", freq_scale, "signal" )
         .connect(io, "knob_1", res_scale, "signal" )
         .connect(freq_scale, "signal", filter, "frequency")
         .connect(res_scale,  "signal", filter, "resonance")
         .path(osc, filter, gain, invert)
         .connect(env, "signal", osc, "amplitude")
         .connect(gain, "signal", audio, "left")
         .connect(gain, "signal", audio, "right")
         .add(player)
         .add(timer);

    env.set_adsr(0.5, 1.0, 1.0, 0.1);

    player.set({ C1, G1, C2, DS2, G2, F2, C3, AS2, DS1, D1 }, [&] (double freq) {
      osc.set_input("frequency", freq);
      env.trigger();
    }, 0.1);

    timer.set(0.1, [&]() {
        std::cout << "\rFreq: "
                  << freq_scale.get_output("signal")
                  << ", Res: "
                  << res_scale.get_output("signal")
                  << "                        "
                  << std::flush;
        timer.reset();
    });

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}
