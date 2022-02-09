# YASE: Yet Another Synthesizer Engine

## Building

To date, Yase has been developed on MacOS 11.6.1. The following are required:
- 'git', 'g++', and 'make', part of the MacOS Development Tools.
- Brew, for installing things: https://brew.sh/
- PortAudio, which can be installed with 'brew install portaudio'
- RtMidi:, which can be installed with 'brew install rtmidi'
- Google Test: which can be installed with 'brew install googletest'
- Niels Lohman's JSON library, which should be unpacked and put somewhere on your computer. You should then edit the Makefiles to point to the right directory. https://github.com/nlohmann/json

On my Mac, things are installed in strange places. Doing the following helps the linker work right.
```
export PATH="/usr/bin:$PATH"
```

Once all the prerequisites are installed, get the code and compile it

```bash
git clone https://github.com/klavins/yase.git
cd yase
make
```

Assuming no errors, you should be able to hear sound if you run
```bash
./examples/bin/sine_wave
```
Note: I recommend that when you work on audio projects you keep your volume very low until you are sure your code works and isn't producing ear-splitting, speaker cone damaging signals.
