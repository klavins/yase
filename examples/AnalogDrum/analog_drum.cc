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
    Saw bass_oscillator;
    Triangle lead_oscillator;
    Voice bass(bass_oscillator), lead(lead_oscillator);
    Audio audio;
    Container synth;
    Player kick_player, snare_player, bass_player, lead_player;
    Transform invert([](double u) { return -u; });
    Timer timer;
    AutoLoad parameters("parameters.json");
  
    Mix mix({
      {kick,   "signal", 1.25, 0.75},
      {snare,  "signal", 0.75, 1.25},
      {bass,   "signal", 1.25, 0.75},
      {lead,   "signal", 1.00, 0.00},
      {invert, "signal", 0.00, 1.00}
    });

    mix.connect(lead,invert)  
       .connect(parameters, "kick", kick)
       .connect(parameters, "snare", snare)
       .connect(parameters, "bass", bass)
       .connect(parameters, "lead", lead);

    synth.add(kick_player)
         .add(snare_player)
         .add(bass_player)
         .add(lead_player)
         .connect(mix, "left", audio, "left")
         .connect(mix, "right", audio, "right");

    kick_player.set  ({ 
      2,0,0,0,2,0,0,0,
      2,0,0,0,2,0,0,1
    }, [&] (double vol) {
        if ( vol > 0 ) {
          kick.set_input("amplitude", 1*vol);
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
