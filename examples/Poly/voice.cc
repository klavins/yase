// 
// YASE Example
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

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

    controls.map(osc, "morph", 0, 1, config["osc1"]["morph"]);

    controls.map(env, "attack",  0.005, 1, config["eg1"]["A"]);
    controls.map(env, "decay",   0.005, 1, config["eg1"]["D"]);
    controls.map(env, "sustain", 0,     1, config["eg1"]["S"]);
    controls.map(env, "release", 0.005, 1, config["eg1"]["R"]);

    controls.map(filter, "frequency", 1000, 6000, config["lpf"]["frequency"]);
    controls.map(filter, "resonance", 0.1, 20, config["lpf"]["resonance"]);    

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

