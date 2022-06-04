#include <iostream>
#include <stdio.h>
#include "yase.hh"
#include "voice.hh"

using namespace yase;

int main(int argc, char * argv[]) {

     int n = 8;

     json config = get_config("./examples/Poly/config.json");

     Container synth;
     Audio audio;
     MidiInput midi_keyboard(config["keyboard"]),
              midi_controller(config["controller"]);
     Controls controls;
     Mixer mixer(n);
     Gain gain;
     Echo echo;

     vector<Voice *> voices;

     for ( int i=0; i<n; i++ ) {
        voices.push_back(new Voice(config));
     }

     synth.add(audio)
          .add(gain)
          .add(controls)
          .add(mixer)
          .add(midi_keyboard)
          .add(midi_controller)   
          .add(echo)                
          .propagate_to(controls);

     vector<tuple<int,Voice *>> notes;

     auto find_note = [&](int id) {
          int i;
          for ( i=0; i < n && std::get<0>(notes[i]) != id;  i++ );
          return i;
     };

     for ( int i=0; i<n; i++ ) {
        synth.add(*voices[i])
             .propagate_to(*voices[i]);
        notes.push_back({0,&(*voices[i])});
        synth.connect(*voices[i], "signal", mixer, i);        
     }

     synth.connect( mixer, "signal", echo, "signal")
          .connect( echo, "signal", gain, "signal" )
          .connect( gain, "signal", audio, "left")
          .connect( gain,  "signal", audio, "right");

     synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
           if ( e.port == midi_keyboard.port() ) {

             Voice * note;
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
               Voice * note = std::get<1>(notes[i]);
               note->keyup(e);
             }
           }
      });

     controls.map(gain, "amplitude", 0, 0.1, config["volume"]);
     controls.map(echo, "duration", 0.001 * SAMPLE_RATE, SAMPLE_RATE, config["echo"]["duration"]);
     controls.map(echo, "amplitude", 0, 0.99, config["echo"]["amplitude"]);

     synth.run(UNTIL_INTERRUPTED);

     return 0;

}