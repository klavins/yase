#include <iostream>
#include "yase.hh"

using namespace yase;

#define MOD(__tuple__)  std::get<0>(__tuple__)
#define PORT(__tuple__) std::get<1>(__tuple__)

int main(int argc, char * argv[]) {

    // READ CONFIG FILE
    json config = get_config("./examples/Modular/config.json");    

    // DECLARE MODULES
    Container synth;
    Audio audio(2,1);
    MidiInput keyboard(config["keyboard"]);
    MidiInput controller(config["controller"]);
    MidiInput matrix_input(config["matrix_input"]); 
    Buttons matrix_output(config["matrix_output"]); 
    Controls controls;
    OscGroup osc1, osc2;
    Sine lfo;
    Biquad lpf;
    Envelope eg1, eg2;
    Echo echo;
    Gain amplifier, volume;
    Mixer mixer[8] = { 8, 8, 8, 8, 8, 8, 8, 8 };

    // ADD MODULES TO SYNTH
    synth.add(audio)
         .add(keyboard)
         .add(controller)
         .add(matrix_input)
         .add(matrix_output)
         .add(controls)
         .add(osc1)
         .add(osc2)
         .add(lfo)
         .add(lpf)
         .add(eg1)
         .add(eg2)
         .add(echo)
         .add(amplifier)
         .add(volume)
         .propagate_to(matrix_output)
         .propagate_to(controls);

    //synth.connect(osc1, "signal", amplifier, "signal");
    amplifier.set_input("amplitude", 1.0);

    for ( int i=0; i<8; i++ ) {
        synth.add(mixer[i]);
    }         

    // DECLARE INPUT AND OUTPUT VECTORS
    vector<tuple<Module *, string> > inputs, outputs;    

    // ORGANIZE INPUTS
    inputs.push_back({&osc1,      "modulation"});
    inputs.push_back({&osc2,      "modulation"});
    inputs.push_back({&lfo,       "frequency"});
    inputs.push_back({&lpf,       "signal"});
    inputs.push_back({&lpf,       "frequency"});
    inputs.push_back({&echo,      "signal"});
    inputs.push_back({&amplifier, "signal"});
    inputs.push_back({&amplifier, "amplitude"});

    // ORGANIZE OUTPUTS
    outputs.push_back({&osc1,   "signal"});
    outputs.push_back({&osc2,   "signal"});
    outputs.push_back({&lfo,    "signal"});
    outputs.push_back({&lpf,    "signal"});
    outputs.push_back({&eg1,    "signal"});
    outputs.push_back({&eg2,    "signal"});
    outputs.push_back({&echo,   "signal"});
    outputs.push_back({&audio,  "line_in0"});

    // CONNECT OUTPUTS TO INPUT MIXERS
    for ( int i=0; i<inputs.size(); i++ ) {
        for ( int j=0; j<outputs.size(); j++ ) {
            if ( ! ( ( i == 2 || i == 4 || i == 7 ) && j == 7 ) ) {
              synth.connect(*MOD(outputs[j]), PORT(outputs[j]), mixer[i], j);
              mixer[i].set_amplitude_input(j,0);
            }
        }
        synth.connect(mixer[i], "signal", *MOD(inputs[i]), PORT(inputs[i]));
    }

    // CONNECT VOLUME TO AUDIO
    synth.connect(amplifier, "signal", volume, "signal")
         .connect(volume,    "signal", audio,  "left")
         .connect(volume,    "signal", audio,  "right");

    // OSCILLATOR KNOBS
    synth.listen(MIDI_MOD, [&] (const Event &e) {
        if ( e.id == config["osc1"]["select"] ) osc1.select(e.value / 127.0); 
    });
    synth.listen(MIDI_MOD, [&] (const Event &e) {
        if ( e.id == config["osc2"]["select"] ) osc2.select(e.value / 127.0); 
    });
    controls.map(osc1, "tuning",   -7, 8, config["osc1"]["tuning"]);
    controls.map(osc1, "harmonic", -2, 3, config["osc1"]["harmonic"]);
    controls.map(osc2, "tuning",   -7, 8, config["osc2"]["tuning"]);
    controls.map(osc2, "harmonic", -2, 3, config["osc2"]["harmonic"]);

    // LFO KNOBS
    controls.map(mixer[2], 7, 0, 1, config["lfo"]["offset"]);
    mixer[2].set_amplitude_input(7,1);
    controls.map(mixer[2], "output_gain", 0.01, 10, config["lfo"]["frequency"]);
    controls.map(lfo, "amplitude", 0, 1,    config["lfo"]["amplitude"]);

    // LPF KNOBS
    //mixer[4].set_input(7,1);
    controls.map(mixer[4], 7, 0, 1, config["lpf"]["offset"]);
    mixer[4].set_amplitude_input(7,1);
    controls.map(lpf, "resonance", 0.1, 20, config["lpf"]["resonance"]);
    controls.map(mixer[4], "output_gain", 10, 6000, config["lpf"]["frequency"]);

    // EG1 KNOBS
    controls.map(eg1, "attack",  0.005, 1, config["eg1"]["A"]);
    controls.map(eg1, "decay",   0.005, 1, config["eg1"]["D"]);
    controls.map(eg1, "sustain", 0,     1, config["eg1"]["S"]);
    controls.map(eg1, "release", 0.005, 1, config["eg1"]["R"]);    

    // EG2 KNOBS
    controls.map(eg2, "attack",  0.005, 1, config["eg2"]["A"]);
    controls.map(eg2, "decay",   0.005, 1, config["eg2"]["D"]);
    controls.map(eg2, "sustain", 0,     1, config["eg2"]["S"]);
    controls.map(eg2, "release", 0.005, 1, config["eg2"]["R"]);

    // ECHO KNOBS
    controls.map(echo, "duration", 0.001 * SAMPLE_RATE, SAMPLE_RATE, config["echo"]["duration"]);
    controls.map(echo, "amplitude", 0, 0.99, config["echo"]["amplitude"]);

    // VCA KNOB
    controls.map(mixer[7], 7, 0, 1, config["vca"]["offset"]);

    // VOLUME KNOB
    controls.map(volume, "amplitude", 0, 0.25, config["volume"]);

    // DEFINE MATRIX BEHAVIOR
    matrix_output.send({240,0,32,41,2,13,0,127,247}); // set launchpad to programmer mode   
    int colors[5] = { 0, 11, 10, 9, 5 };
    for(int output=0; output<8; output++) {
        for ( int input=0; input<8; input++ ) {
            if ( ! ( ( input == 2 || input == 4 || input == 7 ) && output == 7 ) ) {
                synth.listen(MIDI_KEYDOWN, matrix_input.port(), [&,input,output] (const Event &e) {
                    if ( e.id == config["buttons"][output][input] && e.value > 0 ) {
                        double value = mixer[input].get_amplitude_input(output);
                        value += 0.2;
                        if ( value >= 1.0 ) value = 0.0;
                        mixer[input].set_amplitude_input(output, value);
                        matrix_output.on(e.id, colors[(int) (5*value)]);
                    }
                });
            }
        }
    }

    // SET UP KEYBOARD
    std::vector<int> keys;

    synth.listen(MIDI_KEYDOWN, keyboard.port(), [&] (const Event &e) {
        osc1.set_input("frequency", e.frequency());
        osc2.set_input("frequency", e.frequency());
        lpf.set_input("offset", e.frequency());
        lpf.recalculate(); 
        eg1.set_input("velocity", e.value / 127.0);
        eg1.trigger();
        eg2.set_input("velocity", e.value / 127.0);
        eg2.trigger();        
        keys.push_back(e.id);
     })

    .listen(MIDI_KEYUP, keyboard.port(), [&] (const Event &e) {
        keys.erase(std::remove(keys.begin(), keys.end(), e.id), keys.end());
        if ( keys.size() == 0 ) {
            eg1.release();
            eg2.release();
        } else {
            Event temp(MIDI_KEYDOWN, keys.back(), 0, 0);
            osc1.set_input("frequency", temp.frequency());
            osc2.set_input("frequency", temp.frequency());
            lpf.set_input("offset", temp.frequency());
            lpf.recalculate(); 
        }
    });    

    // GO!
    synth.run(UNTIL_INTERRUPTED);

    matrix_output.clear_leds();    

    return 0; 

}