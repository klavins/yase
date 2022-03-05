#include "voice.hh"
#include "yase.hh"

namespace yase {

  Voice::Voice(json &config) :
        Container(),
        osc("./wavetables/i-o.wav")
  {

    signal = add_output("signal");

    filter.set_type("lpf");   

    add(osc);
    add(env);
    add(filter);
    add(controls);
    propagate_to(controls);

    connect( osc,    "signal", filter, "signal");
    connect( filter, "signal", env,    "signal");

    controls.control(osc, "morph", 0, 1, config["osc1"]["morph"]);

    controls.control(env, "attack",  0.005, 1, config["eg1"]["A"]);
    controls.control(env, "decay",   0.005, 1, config["eg1"]["D"]);
    controls.control(env, "sustain", 0,     1, config["eg1"]["S"]);
    controls.control(env, "release", 0.005, 1, config["eg1"]["R"]);

    controls.control(filter, "frequency", 1000, 6000, config["lpf"]["frequency"]);
    controls.control(filter, "resonance", 0.1, 20, config["lpf"]["resonance"]);    

  }

  void Voice::init() {
    Container::init();    
  }

  void Voice::update() {
    Container::update();
    env.copy_outputs(*this);
  }    

  void Voice::keydown(const Event &e) {

    osc.set_input("frequency", e.frequency());
    filter.set_input("offset", e.frequency());
    filter.recalculate(); 
    env.set_input("velocity", e.value / 127.0);
    env.trigger();

  }

  void Voice::keyup(const Event &e) {

    env.release();

  }  

}

