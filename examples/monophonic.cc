#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    // Components
    Synthesizer synth; 
    OscGroup osc[3];
    Sine lfo;
    Mixer mixer(3),
          mod_mixer1(2),
          mod_mixer2(2),
          filter_env_mixer(2);
    Audio audio;
    Midi midi;
    Envelope env, filter_env;
    Biquad filter;
    Gain gain,
         osc2_lfo_gain;
    Sequencer seq;
    
    synth.add(osc[0])    .add(osc[1])      .add(osc[2])
         .add(mixer)     .add(mod_mixer1)  .add(mod_mixer2)
         .add(audio)     .add(midi)        .add(lfo)
         .add(env)       .add(filter)      .add(gain)
         .add(osc2_lfo_gain) 
         .add(filter_env_mixer)
         .add(filter_env)
         .add(seq);

    // Connections 
    for ( int i=0; i<3; i++ ) {
         synth.connect(osc[i], "signal", mixer, i);
    }
    synth.connect( mixer,  "signal", filter, "signal")
         .connect( filter, "signal", env,    "signal")
         .connect( env,    "signal", gain,   "signal")
         .connect( gain,   "signal", audio,  "left")
         .connect( gain,   "signal", audio,  "right");

    synth.connect( lfo,    "signal", mod_mixer1, 0 )
         .connect( osc[0], "signal", mod_mixer1, 1 )
         .connect( mod_mixer1, "signal", osc[1], "modulation" )

         .connect( lfo,    "signal", mod_mixer2, 0 )
         .connect( osc[1], "signal", mod_mixer2, 1 )
         .connect( mod_mixer2, "signal", osc[2], "modulation" );

     synth.connect( lfo, "signal", osc2_lfo_gain, "signal" )
          .connect( osc2_lfo_gain, "signal", osc[0], "modulation" );

     synth.connect( filter_env, "signal", filter_env_mixer, 1 )
          .connect( filter_env_mixer, "signal", filter, "frequency" );
     
     filter_env_mixer.set_input(0, 1);

    // Keyboard Control
    int num_keys = 0;
    int keyboard_port = midi.get_port_id("Arturia KeyStep 32");
    synth.listen(MIDI_KEYDOWN, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               for ( int i=0; i<3; i++ ) {
                   osc[i].set_input("frequency", e.frequency());
               }
               filter.set_input("offset", e.frequency());
               filter.recalculate(); // TODO: filter could intercept set_input
               env.set_input("velocity", e.value / 127.0);
               env.trigger();
               filter_env.trigger();
               num_keys++;
          }
     })
     .listen(MIDI_KEYUP, [&] (const Event &e) {
          if ( e.port == keyboard_port ) {
               num_keys--;
               if ( num_keys == 0 ) {
                    env.release();
                    filter_env.release();
               }
          }
     });

     // Oscillator selector control
     int selector_ids[] = { 16, 17, 18 };
     for ( int i=0; i<3; i++ ) {
         synth.listen(MIDI_MOD, [&,i] (const Event &e) {
             if ( e.id == selector_ids[i] ) osc[i].select(e.value / 127.0);
         });
     }

     // Faders
     synth.control(lfo, "frequency", 0.01, 10, 58)            // LFO
          .control(lfo, "amplitude", 0, 10, 54)

          .control(mod_mixer1, 2, 0, 10, 51)                  // OSC CONNECTIONS 
          .control(mod_mixer1, 3, 0, 10, 46)
          .control(mod_mixer2, 2, 0, 10, 52)
          .control(mod_mixer2, 3, 0, 10, 47)
          .control(osc2_lfo_gain, "amplitude", 0, 10, 50)

          .control(filter, "resonance", 0.1, 20, 60)          // FILTER

          .control(filter_env, "attack", 0.005, 1, 49)
          .control(filter_env, "decay", 0.005, 1, 53)
          .control(filter_env, "sustain", 0, 1, 57)
          .control(filter_env, "release", 0.005, 1, 61)

          .control(filter_env_mixer, 2, 1000, 6000, 56 )
          .control(filter_env_mixer, 3, 1000, 6000, 59 )

          .control(env, "attack", 0.005, 1, 19)                 // ENVELOPE
          .control(env, "decay", 0.005, 1, 23)
          .control(env, "sustain", 0, 1, 27)
          .control(env, "release", 0.005, 1, 31)

          .control(gain, "amplitude", 0, 0.25, 62);              // VOLUME

     // Oscillator amplitudes and tuning
     int amplitude_ids[] = { 28, 29, 30},
         tuning_ids[] { 24, 25, 26 },
         harmonic_ids[] = { 20, 21, 22 };
     for (int i=0; i<3; i++) {
         synth.control(mixer, i+3, 0, 1, amplitude_ids[i])
              .control(osc[i], "tuning", -7, 8, tuning_ids[i])
              .control(osc[i], "harmonic", -2, 3, harmonic_ids[i]);
     }

     // MIDI Buttons
     int akai_port = midi.get_port_id("MIDI Mix"),
                     lpf_led = 16,
                     hpf_led = 19,
                     filter_led = 22;

     midi.on(akai_port, lpf_led)
         .on(akai_port, filter_led)
         .off(akai_port, hpf_led);

     synth.button(akai_port, 16, [&] (const Event &e) { 
              filter.set_type("lpf");   
              midi.on(akai_port, lpf_led)
                  .off(akai_port, hpf_led);
          })
          .button(akai_port, 19, [&] (const Event &e) {
              filter.set_type("hpf");
              midi.off(akai_port, lpf_led)
                  .on(akai_port, hpf_led);
          })
          .button(akai_port, 22, [&] (const Event &e) {
              if ( filter.toggle() ) {
                   midi.on(akai_port, filter_led);
              } else {
                   midi.off(akai_port, filter_led);
              }
          });

     // Sequencer setup

     int reset_led = 1,
         record_led = 4,
         stop_led = 7,
         play_led = 10,
         clear_led = 13;

     synth.listen(MIDI_KEYDOWN, [&] (const Event &e) { 
             if ( e.port == keyboard_port ) {
                  seq.keydown(e); 
             }
          })
          .listen(MIDI_KEYUP, [&] (const Event &e) { 
             if ( e.port == keyboard_port ) {
                 seq.keyup(e); 
             }
          })
          .button(akai_port, 27, [&] (const Event &e) {
               seq.insert_rest();
          })
          .button(akai_port, 1, [&] (const Event &e) { 
               seq.reset(); 
          })
          .button(akai_port, 4, [&] (const Event &e) { 
               seq.record();
               midi.on(akai_port, record_led)
                   .off(akai_port, play_led);
          })
          .button(akai_port, 7, [&] (const Event &e) { 
               seq.stop(); 
               midi.off(akai_port, record_led)
                   .off(akai_port, play_led);
          })
          .button(akai_port, 10, [&] (const Event &e) { 
               seq.play(); 
               midi.off(akai_port, record_led)
                   .on(akai_port, play_led);
          })
          .button(akai_port, 13, [&] (const Event &e) { 
               seq.clear(); 
          })
          .button(akai_port, 3, [&] (const Event &e) {
               // decrease rate
               double tempo = seq.get_input("tempo");
               if ( tempo > 20 ) {
                    seq.set_input("tempo", tempo - 20);
               }
          })
          .button(akai_port, 6, [&] (const Event &e) {
               // increase rate
               double tempo = seq.get_input("tempo");
               seq.set_input("tempo", tempo + 20);               
          })
          .button(akai_port, 9, [&] (const Event &e) {
               // decrease duration
               double dur = seq.get_input("duration");
               if ( dur > 0.2 ) {
                    seq.set_input("duration", dur - 0.1);
                    DEBUG
               }
          })
          .button(akai_port, 12, [&] (const Event &e) {
               // increase duration
               double dur = seq.get_input("duration");
               if ( dur < 0.9 ) {
                    seq.set_input("duration", dur + 0.1);
                    DEBUG
               }
          });          


     // Go!
     synth.run(UNTIL_INTERRUPTED);

     // Shutdown
     midi.off(akai_port, lpf_led)
         .off(akai_port, filter_led)
         .off(akai_port, hpf_led);

     return 0;

}