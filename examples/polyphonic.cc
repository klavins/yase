#include <iostream>
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
         .propagate_to(controls);

     vector<tuple<int,Simple *>> notes;

     for ( int i=0; i<n; i++ ) {
        synth.add(*simple[i])
             .propagate_to(*simple[i]);
        synth.connect(*simple[i], "signal", mixer, i);
        notes.push_back({0,&(*simple[i])});
     }
     DEBUG
     synth.connect( mixer, "signal", gain, "signal")
          .connect( gain,   "signal", audio,  "left")
          .connect( gain,   "signal", audio,  "right");

     auto show = [&]() {
          for ( auto n : notes ) {
               std::cout << std::get<0>(n) << "\t";
          }
          std::cout << "\n";
     };

     //! \todo Fix this so that release notes that are still ringing do not get 
     //! \todo reassigned when a new note is pressed (e.g. prioritize notes that
     //! \todo are release *and* not ringing over ones that are released and ringing).

     synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
           if ( e.port == midi_keyboard.port() ) {
             Simple * note = std::get<1>(notes.back());
             notes.pop_back();
             note->keydown(e);
             notes.insert(notes.begin(), {e.id, note});
             show();
           }
      })
      .listen(MIDI_KEYUP, [&] (const Event &e) {
           if ( e.port == midi_keyboard.port() ) {
             int i=0;
             while ( std::get<0>(notes[i]) != e.id ) {
                  i++;
             }
             if ( i < n ) {
               Simple * note = std::get<1>(notes[i]);
               note->keyup(e);
               notes.erase(notes.begin() + i);
               notes.push_back({0,note});
               show();              
             }
           }
      });

     controls.control(gain, "amplitude", 0, 0.1, config["ids"]["volume"]);

     synth.run(UNTIL_INTERRUPTED);

     return 0;

}