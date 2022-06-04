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

#include <vector>
#include <iostream>
#include <string>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine;
    Saw add;
    Triangle tri;
    Square square;
    Noise noise;
    Controls controls;

    add.set_type("additive");

    vector<Oscillator *> oscillators = { 
        &sine,
        &add, 
        &tri, 
        &square, 
        &noise };
        
    vector<Fader *> faders;

    int midi_ids[] = { 19, 23, 27, 31, 49, 53, 57 };

    Sum sum(oscillators.size());
    MidiInput midi("MIDI Mix");
    Audio audio;
    Container synth;

    synth.add(sum)
         .add(audio)
         .add(midi)
         .add(controls)
         .propagate_to(controls);

    int i = 0;
    for ( auto osc : oscillators ) {
        Fader * fader = new Fader(0,1);
        faders.push_back(fader);
        synth.add(*osc)
             .add(*fader);

        controls.map(*fader, midi_ids[i])
                .connect(*fader, "value", *osc, "amplitude")
                .connect(*osc, "signal", sum, "signal_" + std::to_string(i));
        fader->set_input("target",0);
        osc->set_input("amplitude",0);
        i++;
    }

    faders[0]->set_input("target", 64);

    synth.connect(sum, "signal", audio, "left");
    synth.connect(sum, "signal", audio, "right");

    synth.run(UNTIL_INTERRUPTED);

    for ( auto f : faders ) {
        delete f;
    }

    return 0;

}

