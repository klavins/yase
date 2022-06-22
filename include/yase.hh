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
#include "util.hh"

// Controllers
#include "event_manager.hh"
#include "buttons.hh"
#include "controls.hh"
#include "midi_input.hh"
#include "fader.hh"

// Connectivity
#include "gain.hh"
#include "sum.hh"
#include "mixer.hh"

// Utility
#include "noop.hh"
#include "timer.hh"
#include "player.hh"
#include "transform.hh"
#include "sampler.hh"
#include "fft.hh"
#include "anti_alias.hh"
#include "pitches.hh"

// Events
#include "event.hh"

// Audio
#include "audio.hh"
#include "sample.hh"

// Oscillators
#include "oscillator.hh"
#include "sine.hh"
#include "saw.hh" 
#include "triangle.hh"
#include "noise.hh"
#include "square.hh"
#include "osc_group.hh"
#include "clock.hh"
#include "wavetable.hh"
#include "impulse.hh"

// Envelopes
#include "envelope.hh"

// Filters
#include "iir_filter.hh"
#include "biquad.hh"
#include "resonator.hh"

// Effects
#include "delay.hh"
#include "fadable_delay.hh"
#include "echo.hh"

#endif