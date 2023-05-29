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

#include "yase.hh"
#include "../potato_audio.hh"
#include "voice.hh"
#include "io.hh"

#define __ -1

using namespace yase;

int main(int argc, char * argv[]) {

    Sample kick("./sounds/kick_03.wav"),
           snare("./sounds/snare.wav");

    Saw bass_oscillator("raw");
    Voice bass(bass_oscillator);

    Wavetable lead("./sounds/lead.wav");
    Envelope lead_env;
    Echo echo;

    PotatoAudio audio;
    Container synth;
    Player kick_player, snare_player, bass_player, lead_player;
    Mixer mixer_left(4), mixer_right(4);
    Transform invert([](double u) { return -u; });
    Timer timer;

    IO io;
    Transform bass_cutoff    ( [] (double u) { return (1-u)*100 + u*2000; } );
    Transform bass_resonance ( [] (double u) { return (1-u)*0.1 + u*10; } );
    Transform bass_sustain   ( [] (double u) { return (1-u)*0.001 + u; } );
    Transform bass_release   ( [] (double u) { return (1-u)*0.001 + u; } );

    bass.configure({
        {"amplitude", 4},
        {"cutoff", 300},
        {"resonance", 0.1},
        {"attack", 0.004},
        {"decay", 0.4},
        {"sustain", 0.2},
        {"release", 0.1}
    });

    echo.configure({
      {"amplitude", 0.75},
      {"duration", 0.6*SAMPLE_RATE}
    });

    lead_env.set_adsr(0.15,0.001,0.0,0.1);

    synth.connect(kick, "left", mixer_left, "signal_0")
         .connect(kick, "right", mixer_right, "signal_0")

         .connect(snare, "left", mixer_left, "signal_1")
         .connect(snare, "right", mixer_right, "signal_1")

         .connect(bass, "signal", mixer_left, "signal_2")
         .connect(bass, "signal", mixer_right, "signal_2")

         .path(lead, lead_env, echo)
         .connect(echo, "signal", mixer_left, "signal_3")
         .connect(echo, "signal", mixer_right, "signal_3")

         .connect(mixer_left, "signal", audio, "left")
         .connect(mixer_right, "signal", audio, "right")

         .connect(io, "knob_0", bass_cutoff, "signal")
         .connect(bass_cutoff, "signal", bass, "cutoff")

         .connect(io, "knob_1", bass_resonance, "signal")
         .connect(bass_resonance, "signal", bass, "resonance")

         .connect(io, "knob_2", bass_sustain, "signal")
         .connect(bass_sustain, "signal", bass, "sustain")

         .connect(io, "knob_3", bass_release, "signal")
         .connect(bass_release, "signal", bass, "release")

         .add(kick_player)
         .add(snare_player)
         .add(bass_player)
         .add(lead_player);

    mixer_left.set_input("gain_0", 1.5);
    mixer_right.set_input("gain_0", 0.5);

    mixer_left.set_input("gain_1", 0.5);
    mixer_right.set_input("gain_1", 1.5);

    mixer_left.set_input("gain_3", 0.25);
    mixer_right.set_input("gain_3", 0.25);

    kick_player.set  ({
      1,0,0,0,1,0,0,0,
      1,0,0,0,1,0,0,0
    }, [&] (double vol) {
        if ( io.switch_state(0) && vol > 0 ) {
          kick.set_input("amplitude", vol);
          kick.trigger();
          io.set_led(0,true);
        } else {
          io.set_led(0,false);
        }
    }, 0.2);

    snare_player.set({
      0,0,1,0,0,0,1,0,
      0,0,2,1,0,0,1,0
    }, [&] (double vol) {
      if ( io.switch_state(1) && vol > 0 ) {
        snare.set_input("amplitude", 0.5*vol);
        snare.trigger();
        io.set_led(1,true);
      } else {
        io.set_led(1,false);
      }
    }, 0.2);

    int hack = false;

    bass_player.set({
      A1, __, __, __, G2, A2, __, __,
      A1, __, __, C3, __, A2, __, __,
      A1, __, __, __, G2, A2, __, __,
      A1, A2, __, C3, __, A2, __, G1
    }, [&] (double f) {
      if ( ( !hack || io.switch_state(2) ) && f > 0 ) {
        bass.set_input("frequency", f);
        bass.trigger();
        hack = true;
        io.set_led(2,true);
      } else {
        bass.release();
        io.set_led(2,false);
      }
    }, 0.2);

    lead_player.set({
       A4, A5, __, __, C4, __, E4, __,
       G4, G5, __, __, B5, __, __, __,
       F4, F5, __, __, A4, __, C5, __,
       E5, __, D5, __, C5, __, B4, __
    }, [&] (double f) {
      if ( io.switch_state(3) && f > 0 ) {
        lead.set_input("frequency", f);
        lead_env.trigger();
        io.set_led(3,true);
      } else {
        lead_env.release();
        io.set_led(3,false);
      }
    }, 0.8);

    timer.set(0.1, [&]() {
        std::cout << "\r" << (int) ( 255 * io.get_output("knob_0") )
                  << "\t" << (int) ( 255 * io.get_output("knob_1") )
                  << "\t" << (int) ( 255 * io.get_output("knob_2") )
                  << "\t" << (int) ( 255 * io.get_output("knob_3") )
                  << "                  "
        << std::flush;
        timer.reset();
    });

    synth.add(timer);

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}
