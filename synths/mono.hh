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

      std::vector<int> keys;

    };

}

#endif
