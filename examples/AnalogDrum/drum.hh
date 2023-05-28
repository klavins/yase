// 
// YASE Tom Module Header
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

#ifndef YASE_DRUM_H
#define YASE_DRUM_H

#include "yase.hh"

namespace yase {

    class Drum : public Container {

    public:

      Drum();
      void init();
      void update();
      void trigger();

    private:

      Triangle raw_osc;
      Noise noise;
      Mixer mixer;
      AntiAlias osc;
      Biquad filter;
      ExpDecay freq;
      Envelope amp;
      Gain gain, mgain;
      
      int ampitude,
          cutoff, 
          resonance, 
          decay_rate,
          decay_from,
          decay_to,
          attack,
          sustain,
          decay,
          release,
          osc_mix,
          noise_mix,
          mod_gain,
          signal;

    };

}

#endif
