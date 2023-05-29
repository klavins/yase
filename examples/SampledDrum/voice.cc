// 
// YASE Voice Module Implementation
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

  Voice::Voice(Module &raw_osc) : osc(raw_osc) {   

    path(osc,filter,echo,gain);
    connect(env, "signal", osc, "amplitude");

    amplitude = add_input("amplitude");
    frequency = add_input("frequency");
    cutoff = add_input("cutoff");
    resonance = add_input("resonance");
    echo_duration = add_input("echo_duration");
    echo_amplitude = add_input("echo_amplitude");
    env_attack = add_input("attack");
    env_sustain = add_input("sustain");
    env_decay = add_input("decay");
    env_release = add_input("release");

    equate_input("amplitude", gain, "amplitude");
    equate_input("frequency", osc, "frequency");
    equate_input("cutoff", filter, "frequency");
    equate_input("resonance", filter, "resonance");
    equate_input("echo_duration", echo, "duration");
    equate_input("echo_amplitude", echo, "amplitude");
    equate_input("attack", env, "attack");
    equate_input("sustain", env, "sustain");
    equate_input("decay", env, "decay");
    equate_input("release", env, "release");

    // defaults
    configure({
        {"amplitude", 0.25},
        {"frequency", 440},
        {"cutoff", 2000},
        {"resonance", 10},
        {"echo_duration", SAMPLE_RATE},
        {"echo_amplitude", 0.01},
        {"attack", 0.001},
        {"sustain", 1},
        {"decay", 0.5},
        {"release", 0.2},
    });

    signal = add_output("signal");
    equate_output("signal", gain, "signal");

  }

  void Voice::trigger() {
    env.trigger();
  }

  void Voice::release() {
    env.release();
  }  

  void Voice::init() {
    Container::init();
  }

  void Voice::update() {
    Container::update();
  }    

}