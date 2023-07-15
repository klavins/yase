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

using namespace yase;

int main(int argc, char * argv[]) {

    json config = get_config("./config.json");

    Sine sine;
    Audio audio;
    MidiToAnalog midi(config["keyboard"]);
    Envelope envelope;
    Transform mult([](double u) { return 100*u; });
    Container synth;

    envelope.set_adsr( 0.01, 0.1, 1.0, 0.5 );
 
    synth.connect(midi,"trigger",envelope,"trigger")
         .connect(envelope,"signal", sine, "amplitude")
         .connect(midi,"frequency", sine, "frequency")
         .connect(midi, "velocity", envelope, "velocity")
         .connect(midi, "aftertouch", mult, "signal")
         .connect(mult, "signal", sine, "tuning")
         .connect(sine,"signal",audio,"left")
         .connect(sine,"signal",audio,"right")
         .add(midi);

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}