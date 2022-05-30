#include "mono.hh"
#include "yase.hh"

namespace yase {

  Mono::Mono(MidiInput &keyboard, MidiInput &controller, json &config) : 
                 Container(), 
                 buttons(config["controller"]),
                 mixer(3),
                 mod_mixer1(2),
                 mod_mixer2(2),
                 filter_env_mixer(2),
                 keyboard(keyboard),
                 controller(controller),
                 config(config)
  {   

    // OUTPUT
    signal = add_output("signal");

    // COMPONENTS
    add(osc[0]);
    add(osc[1]);      
    add(osc[2]);
    add(mixer);     
    add(mod_mixer1);
    add(mod_mixer2);  
    add(lfo);
    add(env);      
    add(filter);     
    add(gain);
    add(osc2_lfo_gain); 
    add(filter_env_mixer);
    add(filter_env);
    add(seq);   
    add(clock); 
    add(noop);
    add(buttons);
    add(controls);
    propagate_to(buttons);
    propagate_to(controls);

    // CONNECTIONS
    for ( int i=0; i<3; i++ ) 
        connect(osc[i], "signal", mixer, i);

    connect( mixer,  "signal", filter, "signal");
    connect( filter, "signal", env,    "signal");
    connect( env,    "signal", gain,   "signal");
    connect( lfo,    "signal", mod_mixer1, 0 );
    connect( osc[0], "signal", mod_mixer1, 1 );
    connect( mod_mixer1, "signal", osc[1], "modulation" );
    connect( lfo,    "signal", mod_mixer2, 0 );
    connect( osc[1], "signal", mod_mixer2, 1 );
    connect( mod_mixer2, "signal", osc[2], "modulation" );
    connect( lfo, "signal", osc2_lfo_gain, "signal" );
    connect( osc2_lfo_gain, "signal", osc[0], "modulation" );
    connect( filter_env, "signal", filter_env_mixer, 1 );
    connect( filter_env_mixer, "signal", filter, "frequency" );
    connect ( clock, "signal", seq, "clock");

    clock.set_input("frequency", 4);

    // DEFAULT INPUT
    filter_env_mixer.set_input(0, 1);    

    // MIDI KEYBOARD LISTENERS
    listen(MIDI_KEYDOWN, keyboard.port(), [&] (const Event &e) { keydown(e); });
    listen(MIDI_KEYUP, keyboard.port(), [&] (const Event &e) { keyup(e); });

    // OSCILLATOR TYPE SELECTIONS
    for ( int i=0; i<3; i++ ) {
        listen(MIDI_MOD, [&,i] (const Event &e) { select(e, i); });
    }    

    // CONTROLS
    json ids = config["ids"];

    // LFO CONTROLS
    controls.control(lfo, "frequency", 0.01, 10, ids["lfo_freq"]);
    controls.control(lfo, "amplitude", 0, 10,    ids["lfo_amp"]);

    // OSCILLATOR MODULATION CONTROLS
    // CHECK: DOES EACH OSCILLATOR HAVE AN LFO GAIN?
    controls.control(mod_mixer1, mod_mixer1.amplitude_index(0), 0, 5, ids["mod_mixer_lfo_ctrl"][0]); 
    controls.control(mod_mixer1, mod_mixer1.amplitude_index(1), 0, 5, ids["mod_mixer_mod_ctrl"][0]);
    controls.control(mod_mixer2, mod_mixer2.amplitude_index(0), 0, 5, ids["mod_mixer_lfo_ctrl"][1]);
    controls.control(mod_mixer2, mod_mixer2.amplitude_index(1), 0, 5, ids["mod_mixer_mod_ctrl"][1]);
    controls.control(osc2_lfo_gain, "amplitude", 0, 10, ids["osc2_lfo_gain"]);

    // OSCILLATOR AMPLITUDES AND MIXING
    for (int i=0; i<3; i++) {
        controls.control(mixer, i+3,          0, 1, ids["amplitudes"][i]);
        controls.control(osc[i], "tuning",   -7, 8, ids["tunings"][i]);
        controls.control(osc[i], "harmonic", -2, 3, ids["harmonics"][i]);
    }        

    // FILTER CONTROLS
    controls.control(filter, "resonance", 0.1, 20, ids["filter_resonance"]);
    controls.control(filter_env, "attack",  0.005, 1, ids["filter_env"]["A"]);
    controls.control(filter_env, "decay",   0.005, 1, ids["filter_env"]["D"]);
    controls.control(filter_env, "sustain", 0,     1, ids["filter_env"]["S"]);
    controls.control(filter_env, "release", 0.005, 1, ids["filter_env"]["R"]);
    controls.control(filter_env_mixer, filter_env_mixer.amplitude_index(0), 10, 6000, ids["filter_freq"] );
    controls.control(filter_env_mixer, filter_env_mixer.amplitude_index(1), 10, 6000, ids["filter_eg_amt"] );

    // MAIN ENVELOPE
    controls.control(env, "attack",  0.005, 1, ids["env"]["A"]);
    controls.control(env, "decay",   0.005, 1, ids["env"]["D"]);
    controls.control(env, "sustain", 0,     1, ids["env"]["S"]);
    controls.control(env, "release", 0.005, 1, ids["env"]["R"]);

    // VOLUME
    controls.control(gain, "amplitude", 0, 0.25, ids["volume"]);

    json button = ids["buttons"];

    // BUTTONS FOR FILTER SELECTION
    buttons.mutex({ids["buttons"]["lpf"], ids["buttons"]["hpf"]}, {
          [&] (const Event &e) { filter.set_type("lpf"); }, 
          [&] (const Event &e) { filter.set_type("hpf"); }
          });

    buttons.toggle(ids["buttons"]["filter_toggle"], [&] (const Event &e) {
              filter.toggle();
          }, true);

    // KILLER RANDOMIZE BUTTON
    buttons.momentary(ids["buttons"]["randomize"], [&] (const Event &e) {
          controls.randomize();
          gain.set_input("amplitude", 0.1);
    });    

    // SEQUENCER KEYBOARD EVENTS
    listen(MIDI_KEYDOWN, keyboard.port(), [&] (const Event &e) { seq.keydown(e); });
    listen(MIDI_KEYUP, keyboard.port(), [&] (const Event &e) { seq.keyup(e); });

    // SEQUENCERS BUTTONS
    buttons.mutex({
        button["stop"], 
        button["record"],
        button["play"] }, {
        [&] (const Event &e) { seq.stop(); },
        [&] (const Event &e) { seq.record(); },
        [&] (const Event &e) { seq.play(); }
    });

    buttons.momentary(button["rest"], [&] (const Event &e) { seq.insert_rest(); });
    buttons.momentary(button["reset"], [&] (const Event &e) { seq.reset(); });
    buttons.momentary(button["clear"], [&] (const Event &e) { seq.clear(); });
    buttons.momentary(button["decrease_duration"], [&] (const Event &e) { seq.decrease_duration(0.1); });
    buttons.momentary(button["increase_duration"], [&] (const Event &e) { seq.increase_duration(0.1); });      

    buttons.momentary(button["decrease_tempo"], [&] (const Event &e) { 
        double f = clock.get_input("frequency");
        if ( f > 0.25 ) {
            clock.set_input("frequency", f - 0.25);
        }
    });

    buttons.momentary(button["increase_tempo"], [&] (const Event &e) { 
        double f = clock.get_input("frequency");
        clock.set_input("frequency", f + 0.25);
    });    

  }

  void Mono::inject(Event e) {
      noop.emit(e);
  }

  void Mono::init() {
      Container::init();
  }

  void Mono::update() {
    Container::update();
    gain.copy_outputs(*this);
  }   

  void Mono::select(const Event &e, int i) {
    if ( e.id == config["ids"]["osc_selectors"][i] ) osc[i].select(e.value / 127.0);     
  } 

  void Mono::keydown(const Event &e) {

        for ( int i=0; i<3; i++ ) {
            osc[i].set_input("frequency", e.frequency());
        }

        filter.set_input("offset", e.frequency());
        filter.recalculate(); 
        env.set_input("velocity", e.value / 127.0);
        env.trigger();
        filter_env.trigger();
        keys.push_back(e.id);

  }

  void Mono::keyup(const Event &e) {

        keys.erase(std::remove(keys.begin(), keys.end(), e.id), keys.end());

        if ( keys.size() == 0 ) {
            env.release();
            filter_env.release();
        } else {
            Event temp(MIDI_KEYDOWN, keys.back(), 0, 0);
            for ( int i=0; i<3; i++ ) {
                osc[i].set_input("frequency", temp.frequency());
            }
            filter.set_input("offset", temp.frequency());
            filter.recalculate(); 
        }

    }

    void Mono::clear_leds() {
        buttons.clear_leds();
    }

}