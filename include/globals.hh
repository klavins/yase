#ifndef YASE_GLOBALS_H
#define YASE_GLOBALS_H

#define FOREVER -1
#define FADER_GAIN 20

#define SAMPLE_RATE         44100
#define TS      (1.0/SAMPLE_RATE)
#define FRAMES_PER_BUFFER     512
#define PA_SAMPLE_TYPE  paFloat32
#define CENT 1.05946
#define LN01 2.30258

#define MIDI_ANY -1
#define MIDI_KEYDOWN 144
#define MIDI_KEYUP 128
#define MIDI_BENDER 224
#define MIDI_MOD 176
#define MIDI_AFTER_TOUCH 208

#define DEBUG std::cout << __FILE__ << ", " << __LINE__ << "\n";

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))()

#define ENV_EPS             0.001

#endif