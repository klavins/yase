#include <iostream>
#include <string>
#include "yase.hh"

#define KICK_ID 100
#define SNARE_ID 101
#define CLOSED_ID 102
#define OPEN_ID 103

using namespace yase;

vector<int> id_set(int a, int b, int c, int d) {
    vector<int> v;
    for(int i=a; i<=b; i++) {
        v.push_back(i);
    }
    for(int i=c; i<=d; i++) {
        v.push_back(i);
    }    
    return v;
}

int main(int argc, char * argv[]) {

    Sample kick("./samples/Trap Kicks/trap-kicks-25.wav"),
           snare("./samples/Trap Snares/trap-snare-14.wav"),
           hat_closed("./samples/Trap Hats/trap-hihat-19.wav"),
           hat_open("./samples/Trap Hats/trap-hihat-14.wav");

    vector<Module *> drums = { &kick, &snare, &hat_closed, &hat_open };
    Sequencer seq[4];

    Audio audio(3);
    Mixer mixer(8);
    Buttons buttons("Launchpad Mini MK3 LPMiniMK3 MIDI In");
    MidiInput midi("Launchpad Mini MK3 LPMiniMK3 MIDI Out");  
    Controls controls;
    Clock clock;
    Container synth; 

    clock.set_input("frequency", 4.0);
    
    synth.add(mixer)
         .add(buttons)
         .add(midi)
         .add(audio)
         .add(clock)
         .add(controls)
         .propagate_to(buttons)
         .propagate_to(controls);

    synth.connect(clock, "signal", audio, "aux1");

    for ( int i=0; i<4; i++ ) { 
        synth.add(*drums[i]);
        synth.add(seq[i]);
        seq[i].allocate(16);
        synth.connect(*drums[i], "left", mixer, 2*i);
        synth.connect(*drums[i], "right", mixer, 2*i+1);
        synth.connect(clock, "signal", seq[i], "clock");
    }

    synth.connect(mixer, "signal", audio, "left")
         .connect(mixer, "signal", audio, "right");

    buttons.send({240,0,32,41,2,13,0,127,247}); // set launchpad to programmer mode   

    auto kick_ids = id_set(51,58,11,18);
    auto snare_ids = id_set(61,68,21,28);
    auto closed_ids = id_set(71,78,31,38);
    auto open_ids = id_set(81,88,41,48);

    for ( int i=0; i<16; i++ ) {
        buttons.toggle(kick_ids[i], [&,i] (const Event &e) {
            if ( seq[0].is_rest(i) ) {
                seq[0].set(i, KICK_ID); 
            } else { 
                seq[0].rest(i);
            }
        }, false);
        buttons.toggle(snare_ids[i], [&,i] (const Event &e) {
            if ( seq[1].is_rest(i) ) {
                seq[1].set(i, SNARE_ID); 
            } else { 
                seq[1].rest(i);
            }
        }, false);   
        buttons.toggle(closed_ids[i], [&,i] (const Event &e) {
            if ( seq[2].is_rest(i) ) {
                seq[2].set(i, CLOSED_ID); 
            } else { 
                seq[2].rest(i);
            }
        }, false);    
        buttons.toggle(open_ids[i], [&,i] (const Event &e) {
            if ( seq[3].is_rest(i) ) {
                seq[3].set(i, OPEN_ID); 
            } else { 
                seq[3].rest(i);
            }
        }, false);                 
    }     

    buttons.momentary(89, [&] (const Event & e) {
        if ( seq[0].is_playing() ) {
            for ( int i=0; i<4; i++ ) seq[i].stop();
            buttons.blink_off(89);
        } else {
            for ( int i=0; i<4; i++ ) 
            seq[i].play();
            buttons.blink_on(89, 2/clock.get_input("frequency"));
        }
    }, MIDI_MOD);

    buttons.momentary(79, [&] (const Event & e) {
        clock.set_input("frequency", clock.get_input("frequency") + 0.25);
    }, MIDI_MOD);

    buttons.momentary(69, [&] (const Event & e) {
        double f = clock.get_input("frequency");
        if ( f > 0.25 ) {
            clock.set_input("frequency", f - 0.25);
        }
    }, MIDI_MOD);

    // Listen to sequencer events
    synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
        switch ( e.id ) {
            case KICK_ID:
              kick.trigger();
              break;
            case SNARE_ID: 
              snare.trigger();
              break;
            case CLOSED_ID:
              hat_closed.trigger();
              hat_open.stop();
              break;
            case OPEN_ID:
              hat_open.trigger();
              break;
        }
    });     

    synth.run(UNTIL_INTERRUPTED); 
    buttons.clear_leds();    

    return 0;

}