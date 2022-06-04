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

#ifndef YASE_MONO_H
#define YASE_MONO_H

#include "json.hpp"
#include "controls.hh"
#include "yase.hh"

namespace yase {

    using namespace nlohmann;  

    class Mono : public Container {

    public:

      Mono(MidiInput &keyboard, MidiInput &controller, json &config);
      void init();
      void update();

      void keydown(const Event &e);
      void keyup(const Event &e);
      void select(const Event &e, int i);
      void init_leds();
      void inject(Event e);
      void clear_leds();

    private:

      int signal;

      OscGroup osc[3];

      Buttons buttons;
      Controls controls;

      Sine lfo;

      Mixer mixer,
            mod_mixer1,
            mod_mixer2,
            filter_env_mixer;

      Envelope env, filter_env;
      Biquad filter;
      Gain gain,
           osc2_lfo_gain;

      Sequencer seq; 

      Noop noop;

      MidiInput &keyboard, &controller;
      json &config;

      Clock clock;

      std::vector<int> keys;

    };

}

#endif
