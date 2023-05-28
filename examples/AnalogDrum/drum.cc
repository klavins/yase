// 
// YASE Tom Module Implementation
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

#include "drum.hh"
#include "yase.hh"

#define INTERP(a,b) ((1-u)*a + u*b)

namespace yase {

  Drum::Drum() :  osc(raw_osc), mixer(2) {

      ampitude = add_input("amplitude");
      cutoff = add_input("cutoff");
      resonance = add_input("resonance");
      decay_rate = add_input("decay_rate");
      decay_from = add_input("decay_from");
      decay_to = add_input("decay_to");
      attack = add_input("attack");
      sustain = add_input("sustain");
      decay = add_input("decay");                  
      release = add_input("release");
      osc_mix = add_input("osc_mix");
      noise_mix = add_input("noise_mix");
      mod_gain = add_input("modulation_gain");

      equate_input("amplitude",  gain, "amplitude");
      equate_input("cutoff",     filter, "frequency");
      equate_input("resonance",  filter, "resonance");
      equate_input("decay_rate", freq, "rate");
      equate_input("decay_from", freq, "from");
      equate_input("decay_to",   freq, "to");
      equate_input("attack",     amp, "attack");
      equate_input("sustain",    amp, "sustain");
      equate_input("decay",      amp, "decay");                  
      equate_input("release",    amp, "release");
      equate_input("osc_mix",    mixer, "gain_0");
      equate_input("noise_mix",  mixer, "gain_1");
      equate_input("modulation_gain", mgain, "amplitude");

      signal = add_output("signal");
      equate_output("signal", gain, "signal");

      connect(osc, "signal", mixer, "signal_0");
      connect(noise, "signal", mixer, "signal_1");
      path(mixer, filter, gain);
      connect(freq, "signal", mgain, "signal");
      connect(mgain, "signal", osc, "modulation");
      connect(freq, "signal", osc, "frequency");
      connect(amp,  "signal", osc, "amplitude");
      connect(amp,  "signal", noise, "amplitude");

      // defaults
      configure({
        {"amplitude", 0.5},
        {"cutoff", 5000},
        {"resonance", 10},
        {"decay_rate", 5},
        {"decay_from",50},
        {"decay_to", 30},
        {"attack", 0.01},
        {"sustain", 0.1},
        {"decay", 0.2},
        {"release", 0.001},
        {"osc_mix", 1},
        {"noise_mix", 0.0},
        {"modulation_gain", 1}
      });


  }

  void Drum::trigger() {
    freq.trigger();
    amp.trigger();
  }

  void Drum::init() {
    Container::init();
  }

  void Drum::update() {
    Container::update();
  }

}