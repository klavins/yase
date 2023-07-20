// 
// YASE Include Everything Header
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

#ifndef YASE_H
#define YASE_H

// Base
#include "globals.hh"
#include "exception.hh"
#include "module.hh"
#include "container.hh"
#include "sequencer.hh"
#include "simple_sequencer.hh"
#include "util.hh"

// Controllers
#include "event_manager.hh"
#include "buttons.hh"
#include "controls.hh"
#include "midi_input.hh"
#include "midi_output.hh"
#include "midi_to_analog.hh"
#include "fader.hh"

// Connectivity
#include "gain.hh"
#include "sum.hh"
#include "mixer.hh"
#include "mix.hh"

// Utility
#include "noop.hh"
#include "timer.hh"
#include "player.hh"
#include "transform.hh"
#include "linear_transform.hh"
#include "sampler.hh"
#include "fft.hh"
#include "anti_alias.hh"
#include "pitches.hh"
#include "auto_load.hh"

// Events
#include "event.hh"
#include "monitor.hh"

// Audio
#ifndef POTATO
  #include "audio.hh"
#endif
#include "sample.hh"

// Oscillators
#include "oscillator.hh"
#include "sine.hh"
#include "saw.hh" 
#include "additive_saw.hh" 
#include "triangle.hh"
#include "additive_triangle.hh"
#include "noise.hh"
#include "square.hh"
#include "additive_square.hh"
#include "osc_group.hh"
#include "clock.hh"
#include "wavetable.hh"
#include "impulse.hh"

// Envelopes
#include "envelope.hh"
#include "exp_decay.hh"

// Filters
#include "iir_filter.hh"
#include "biquad.hh"
#include "resonator.hh"

// Effects
#include "delay.hh"
#include "tapped_delay.hh"
#include "fadable_delay.hh"
#include "echo.hh"
#include "sample_and_hold.hh"

#endif
