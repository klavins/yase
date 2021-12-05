#ifndef YASE_GLOBALS_H
#define YASE_GLOBALS_H

#define SAMPLE_RATE         44100
#define TS      (1.0/SAMPLE_RATE)
#define FRAMES_PER_BUFFER     512
#define PA_SAMPLE_TYPE  paFloat32

#define DEBUG std::cout << __FILE__ << ", " << __LINE__ << "\n";

#endif