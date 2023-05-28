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
#include "drum.hh"
#include "voice.hh"

#define __ -1

using namespace yase;

int main(int argc, char * argv[]) {

    Drum kick, snare;
    Saw bass_oscillator("raw");
    Triangle lead_oscillator;
    Voice bass(bass_oscillator), lead(lead_oscillator);
    Audio audio;
    Container synth;
    Player kick_player, snare_player, bass_player, lead_player;
    Mixer mixer_left(4), mixer_right(4);
    Transform invert([](double u) { return -u; });
    Timer timer;
   
    kick.configure({
      {"cutoff", 2000},
      {"resonance", 1},
      {"decay_rate", 40},
      {"decay_from",60},
      {"decay_to", 30},
      {"attack", 0.001},
      {"sustain", 0.001},
      {"decay", 0.15},
      {"release", 0.01},
      {"osc_mix", 1},
      {"noise_mix", 0.0},
      {"modulation_gain", 0.05}
    });

    snare.configure({
      {"cutoff", 5000},
      {"resonance", 2},
      {"decay_rate", 300},
      {"decay_from", 500},
      {"decay_to", 180},
      {"attack", 0.001},
      {"sustain", 0.0},
      {"decay", 0.14},
      {"release", 0.01},
      {"osc_mix", 0.5},
      {"noise_mix", 0.2},
      {"modulation_gain", 0.1}
    });

    bass.configure({
        {"amplitude", 4},
        {"cutoff", 300},
        {"resonance", 0.1},
        {"attack", 0.004},
        {"decay", 0.4},
        {"sustain", 0.2},                
        {"release", 0.1},
        {"echo_duration", 0.8*SAMPLE_RATE},
        {"echo_amplitude", 0.1}        
    });

    lead.configure({
        {"cutoff", 2000},
        {"resonance", 10},      
        {"attack", 0.2},
        {"decay", 0.01},        
        {"sustain", 0},
        {"release", 0.001},
        {"echo_duration", 0.6 * SAMPLE_RATE},
        {"echo_amplitude", 0.75}
    });

    synth.connect(kick, "signal", mixer_left, "signal_0")
         .connect(kick, "signal", mixer_right, "signal_0")

         .connect(snare, "signal", mixer_left, "signal_1")
         .connect(snare, "signal", mixer_right, "signal_1")   

         .connect(bass, "signal", mixer_left, "signal_2") 
         .connect(bass, "signal", mixer_right, "signal_2")   

         .connect(lead, "signal", mixer_left, "signal_3")
         .connect(lead, invert)
         .connect(invert, "signal", mixer_right, "signal_3")   

         .connect(mixer_left, "signal", audio, "left")
         .connect(mixer_right, "signal", audio, "right")
         
         .add(kick_player)
         .add(snare_player)
         .add(bass_player)
         .add(lead_player);

    mixer_left.set_input("gain_0", 1.5);
    mixer_right.set_input("gain_0", 0.5);

    mixer_left.set_input("gain_1", 0.5);
    mixer_right.set_input("gain_1", 1.5);    

    kick_player.set  ({ 
      1,0,0,1,0,1,0,0,
      2,1,0,0,1,0,0,0
    }, [&] (double vol) {
        if ( vol > 0 ) {
          kick.set_input("amplitude", 4*vol);
          kick.trigger();
        }
    }, 0.2);

    snare_player.set({ 
      0,0,1,0,0,0,1,0,
      0,0,2,1,0,0,1,0  
    }, [&] (double vol) {
      if ( vol > 0 ) {
        snare.set_input("amplitude", 0.5*vol);
        snare.trigger();
      }
    }, 0.2);    

    bass_player.set({ 
      A1, __, __, __, G2, A2, __, __,
      A1, __, __, C3, __, A2, __, __,
      A1, __, __, __, G2, A2, __, __,
      A1, A2, __, C3, __, A2, __, G1
    }, [&] (double f) {
      if ( f > 0 ) {
        bass.set_input("frequency", f);
        bass.trigger();
      } else {
        bass.release();
      }
    }, 0.2);

    lead_player.set({ 
       A4, A5, __, __, C4, __, __, __,  
       G4, B5, __, __, __, __, __, __,    
       F4, __, D3, __, __, __, __, __, 
       C4, __, B3, __, G3, __, __, __, 
    }, [&] (double f) {
      if ( f > 0 ) {
        lead.set_input("frequency", f);
        lead.trigger();
      } else {
        lead.release();
      }
    }, 0.8);    

    synth.run(UNTIL_INTERRUPTED);

    return 0;

}
