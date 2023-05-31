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
    Transform invert([](double u) { return -u; });
    Timer timer;
   
    kick.configure({
      {"cutoff", 20000},
      {"resonance", 10},
      {"decay_rate", 300},
      {"decay_from",80},
      {"decay_to", 55},
      {"attack", 0.001},
      {"sustain", 0.01},
      {"decay", 0.1},
      {"release", 0.01},
      {"osc_mix", 1},
      {"noise_mix", 0.0},
      {"modulation_gain", 0.1}
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
      {"noise_mix", 0.1},
      {"modulation_gain", 0.1}
    });

    bass.configure({
        {"amplitude", 4},
        {"cutoff", 400},
        {"resonance", 0.1},
        {"attack", 0.004},
        {"decay", 0.3},
        {"sustain", 0.2},                
        {"release", 0.02},
        {"echo_duration", 0.2*SAMPLE_RATE},
        {"echo_amplitude", 0.1}        
    });

    lead.configure({
        {"cutoff", 4000},
        {"resonance", 10},      
        {"attack", 0.2},
        {"decay", 0.01},        
        {"sustain", 0},
        {"release", 0.11},
        {"echo_duration", 0.6 * SAMPLE_RATE},
        {"echo_amplitude", 0.75}
    });

    Mix mix({
      {kick,   "signal", 1.75, 0.25},
      {snare,  "signal", 1.00, 1.00},
      {bass,   "signal", 0.25, 0.25}
    });

    mix.connect(lead, invert);
    mix.extend(lead, "signal", 1.0, invert, "signal", 1.0);

    synth.add(mix)       
         .add(kick_player)
         .add(snare_player)
         .add(bass_player)
         .add(lead_player)
         .connect(mix, "left", audio, "left")
         .connect(mix, "right", audio, "right");

    kick_player.set  ({ 
      2,0,0,0,2,0,0,0,
      2,0,0,1,2,0,0,0
    }, [&] (double vol) {
        if ( vol > 0 ) {
          kick.set_input("amplitude", 1.5*vol);
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
       A4, A5, __, __, C4, __, E4, __,  
       G4, G5, __, __, B5, __, __, __,    
       F4, F5, __, __, A4, __, C5, __, 
       E5, __, D5, __, C5, __, B4, __
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
