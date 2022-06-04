/*! \file */

// 
// YASE Globals
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

#ifndef YASE_GLOBALS_H
#define YASE_GLOBALS_H

//! Use as in Synthesizer::run(UNTIL_INTERUPTED)
#define UNTIL_INTERRUPTED -1
//! The gain in the proportional control of fader target tracking
#define FADER_GAIN 20

//! The sample rate of everything in yase
#define SAMPLE_RATE         44100
//! The duration of a sample
#define TS      (1.0/SAMPLE_RATE)
//! The number of frames to prepare to be sent to the audio interface
#define FRAMES_PER_BUFFER     128
//! Use floats for the audio interface
#define PA_SAMPLE_TYPE  paFloat32
//! A unit of pitch
#define CENT 1.05946
#define LN01 2.30258

//! Any MIDI event
#define MIDI_ANY -1
//! A key press
#define MIDI_KEYDOWN 144
//! A key release
#define MIDI_KEYUP 128
//! The bender
#define MIDI_BENDER 224
//! A fader or knob
#define MIDI_MOD 176
//! After touch for those keyboards that have it
#define MIDI_AFTER_TOUCH 208
//! Used by sequencers when constructing events that should be interpreted as rests
#define SEQUENCE_REST -1

//! Useful for debugging
#define DEBUG std::cout << __FILE__ << ", " << __LINE__ << "\n";

//! Call a member function. Used in, for example, the envelope generator
#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))()

//! Amplitude below which an envelope is considered off
#define ENV_EPS             0.001

#endif