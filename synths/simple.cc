#include "simple.hh"
#include "yase.hh"

namespace yase {

  Simple::Simple(json &config) :
        Container()
  {

    signal = add_output("signal");

    filter.set_type("lpf");   
    osc.set_type("additive");

    add(osc);
    add(env);
    add(filter);
    add(controls);
    propagate_to(controls);

    connect( osc,    "signal", filter, "signal");
    connect( filter, "signal", env,    "signal");

    json ids = config["ids"];
    controls.control(env, "attack",  0.005, 1, ids["env"]["A"]);
    controls.control(env, "decay",   0.005, 1, ids["env"]["D"]);
    controls.control(env, "sustain", 0,     1, ids["env"]["S"]);
    controls.control(env, "release", 0.005, 1, ids["env"]["R"]);

    controls.control(filter, "frequency", 1000, 6000, ids["filter_freq"]);
    controls.control(filter, "resonance", 0.1, 20, ids["filter_resonance"]);    

  }

  void Simple::init() {
    Container::init();
  }

  void Simple::update() {
    Container::update();
    env.copy_outputs(*this);
  }    

  void Simple::keydown(const Event &e) {

    osc.set_input("frequency", e.frequency());
    filter.set_input("offset", e.frequency());
    filter.recalculate(); 
    env.set_input("velocity", e.value / 127.0);
    env.trigger();

  }

  void Simple::keyup(const Event &e) {

    env.release();

  }  

}

