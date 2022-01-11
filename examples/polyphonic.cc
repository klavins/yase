#include <iostream>
#include <stdio.h>
#include "yase.hh"
#include "simple.hh"

using namespace yase;

int main(int argc, char * argv[]) {

     int n = 8;

     json config = get_config("config/akai-monophonic.json");

     Container synth;
     Audio audio;
     MidiInput midi_keyboard(config["keyboard"]),
              midi_controller(config["controller"]);
     FaderManager controls;
     Mixer mixer(n);
     Gain gain;
     Delay delay;
     Sum sum(2);
     Gain delay_gain;

     vector<Simple *> simple;

     for ( int i=0; i<n; i++ ) {
        simple.push_back(new Simple(config));
     }     

     synth.add(audio)
         .add(gain)
         .add(controls)
         .add(mixer)
         .add(midi_keyboard)
         .add(midi_controller)   
         .add(delay)     
         .add(sum)
         .add(delay_gain) 
         .propagate_to(controls);

     vector<tuple<int,Simple *>> notes;

     auto find_note = [&](int id) {
          int i;
          for ( i=0; i < n && std::get<0>(notes[i]) != id;  i++ );
          return i;
     };

     for ( int i=0; i<n; i++ ) {
        synth.add(*simple[i])
             .propagate_to(*simple[i]);
        notes.push_back({0,&(*simple[i])});
        synth.connect(*simple[i], "signal", mixer, i);        
     }

     delay.set_input("size", 0.001 * SAMPLE_RATE);
     delay_gain.set_input("amplitude", 0.5);

     synth.connect( mixer, "signal", sum,  0)
          .connect( delay_gain, "signal", sum, 1)
          .connect( sum, "signal", delay, "signal" )
          .connect( delay, "signal", delay_gain, "signal")
          .connect( sum,  "signal", gain, "signal" )
          .connect( gain, "signal", audio, "left")
          .connect( gain,  "signal", audio, "right");

     synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
           if ( e.port == midi_keyboard.port() ) {

             Simple * note;
             int i = find_note(e.id);
             if ( i < n ) {
               note = std::get<1>(notes[i]);
               notes.erase(notes.begin() + i);
             } else {
               note = std::get<1>(notes.back());
               notes.pop_back();
             }
             note->keydown(e);
             notes.insert(notes.begin(), {e.id, note});

           }
      })
      .listen(MIDI_KEYUP, [&] (const Event &e) {
           if ( e.port == midi_keyboard.port() ) {
             int i = find_note(e.id);
             if ( i < n ) {
               Simple * note = std::get<1>(notes[i]);
               note->keyup(e);
             }
           }
      });

     controls.control(gain, "amplitude", 0, 0.1, config["ids"]["volume"]);
     controls.control(delay, "size", 0.001*SAMPLE_RATE, 1*SAMPLE_RATE, 49);
     controls.control(delay_gain, "amplitude", 0, 0.99, 53);

     synth.run(UNTIL_INTERRUPTED);

     return 0;

}