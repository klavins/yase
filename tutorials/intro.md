Tutorial: Yase Basics
===

A Sawtooth Wave
===

In this tutorial, we will synthesize a patch to illustrate several basic features of yase including oscillators, filters, envelopes, and anti-aliasing. We will build up complexity incrementally starting from a sawtooth wave. Our starting point is to include the Yase library, open the *yase* namespace (so we do not have to prepend Yase class names with yase::), and write a *main* function that does nothing for now. 

```cpp 
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {
    return 0; 
}
```

At this point you should be able to compile and run the above code, but it will not do anything. Next we will fill in the *main* function. Many Yase main functions are divided into three into three sections: declarations, setup, and execution, which we describe in turn.

**Declarations:** First, we declare three Modules that are enough to give us a basic sound. 

```cpp
Container synth;
Audio audio;
Saw saw("raw");
```

The first is a [Container](classyase_1_1_container.html) into which we will put all other syntheiszer components. The second is an [Audio](classyase_1_1_audio.html) module that interfaces with your computer's audio card to produce sound. The third is a [Sawtooth](classyase_1_1_saw.html) waveform module. The "raw" argument to the *Saw* constructor tells yase to return a digital sawtooth wave that simply increases linearly up to a certain point and then resets. Students of digital signal processing will know that such a waveform, when sampled, will produce unwanted frequencies due to aliasing. We'll fix that soon.

**Setup**: Next we connect the output of the sawtooth module to the two inputs of the audio module. 

```cpp
synth.connect(saw, "signal", audio, "left")
     .connect(saw, "signal", audio, "right");
```

Specifcally, these connect the "signal" output of the *saw* module to the "left" and "right" inputs of the *audio* module. Later, each time the *synth* module is updated, it will first copy the output of the *saw* module to the inputs of the audio module, and then call the update methods of the *saw* and *audio* modules. Note the we are using the strings "signal", "left", and "right" to refer to the inputs and outputs of these modules. That may seem ill-advised in what is supposed to be fast C++ code, since string methods are typically slow. But they are only used in the setup phase. Internally, Yase translates these names to integer indices that are used during execution, so everything is as fast as can be. 

Note: Many *Container* methods, like *connect* return a reference to the container so that you can "method chain". The above is the same as writing

```cpp
synth.connect(saw, "signal", audio, "left");
synth.connect(saw, "signal", audio, "right");
```

but slightly more compact. Not having to see *synth* over and over again will looke nicer when we start connecting dozens of modules. 

**Execution**: Finally, we need a *run* statement. 

```cpp
synth.run(UNTIL_INTERRUPTED);
```

It tells the *synth* module to start updating (at the default frequency of 44100 Hz). 

The entire program now looks like this:

```cpp 
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    // Declare
    Container synth;
    Audio audio;
    Saw saw("raw");

    // Setup
    synth.connect(saw, "signal", audio, "left")
         .connect(saw, "signal", audio, "right");  

    // Execute
    synth.run(UNTIL_INTERRUPTED);   

    // Cleanup
    return 0; 

}
```

Compiling and running this example will produce a somewhat annoying, sawtooth wave at 440 Hz (the default). You can stop it by pressing Cntl-C. Pressing Cntl-C will pop out of the *run* method, so that any clean up code can execute. Here we simply return 0 to the OS and exit. 

Defining a Melody
===

It is hard to evaluate a patch out of context, that is, outside of a melody or harmony. With Yase you can supply a melody in various ways, for example by connecting an external sequencer via MIDI. But for our purposes, it will be much simpler and still ullustrative to use a [Player](classyase_1_1_player.html) Module. In the declarations section of the main function, we write:

```cpp
Player player;
```

and then we add it to the *synth* container. Our *connect* statement becomes

```cpp
synth.connect(saw, "signal", audio, "left")
     .connect(saw, "signal", audio, "right")
     .add(player);
```

Note that all modules contained within *synth* need to be added. However, the *saw* and *audio* modules are added implicitly by connecting them within the *synth* module. The *player* module on the other hand does not directly use signals, so we have add it to the *synth* module explicitly so that it's update method is called. 

Now we need to define the behavior of the player. We can do that with the *set* method to the player as follows.

```cpp
player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
    saw.set_input("frequency", freq);
}, 3.0);
```

The *set* method takes three arguments. The first is a vector of frequencies. Here, we are using Yase's names for notes, like *yase::G3* which we can just write as G3 since we have the yase name space open. The third argument is the duration of each note in seconds. And the second argument is a callback method that defines what to do each time the note changes. This argument is a C++ *lambda*, which is a way of specifying a function without naming it. The player will call the callback every three seconds cycling through the frequencies. You can see in the body of the callback the single line that changes the frequency of the sawtooth wave. The entire program is now

```cpp
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    // Declare
    Container synth;
    Audio audio;
    Saw saw("raw");
    Player player;

    // Setup
    synth.connect(saw, "signal", audio, "left")
         .connect(saw, "signal", audio, "right")
         .add(player);

    player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
        saw.set_input("frequency", freq);
    }, 3.0);

    // Execute
    synth.run(UNTIL_INTERRUPTED);   

    // Cleanup
    return 0; 

}
```

Compiling and running the resulting code should now play the melody over and over again --- using our annoying sawtooth wave --- until you press Cntl-C. Try it!

Anti-Aliasing
===

The raw sawtooth waveform we are using does not sound great. One reason is that sampling it at 44100 Hz produces artifacts due to aliasing. These can be heard as a faint buzz. There are a variety of ways to fix this in Yase. One is to build the sawtooth wave from sine waves. Another is to soften the waveform. These each have their pros and cons. Here, we'll wrap an [AntiAlias](classyase_1_1_anti_alias.html) module around the sawtooth module. It samples the sawtooth at twice the normal sample rate, so 88200 Hz, and then applies a low pass filter to cut off frequencies lower than 22050 Hz. The module then downsamples back to 44100 Hz. Thus, frequencies that would have reflected off of the 22050 Nyquist sampling rate to produce a buzz are now significantly attenuated. The result is a noticable improvement. Using this module is as simple as replacing the *saw* declartion with

```cpp
Saw raw("raw");
AntiAlias saw(raw);
```

Nothing else needs to change. Compiling and running this code produces a slightly less annoying sawtooth wave. But it is still a basic sawtooth wave, which not everyone likes. 

Adding an Envelope
===

One reason the sound of the sawtooth is annoying is that it abrubtly starts and abruptly changes when a new note is played. Musical instruments don't do that. Notes start slowly or quickly, but not instantly. And they decay to a sustained level and ring a bit after they are released. In audio synthesis, this is accomplished with an [Envelope](classyase_1_1_envelope.html), which sets an attack, decay, and release time as well as a sustain level. To add an envelope, we declare one:

```cpp
Envelope envelope;
```

and set its attack, decay, sustain, and release parameters, also known as the ADSR of the envelope, in the setup phase:

```cpp
envelope.set_adsr(0.8, 1.0, 0.5, 0.1);
```

The attack of the standard envelope in Yase is linear, so 0.8 means it will take 0.8 s to get to maximum output. The decay and release are exponential. The 1.0 and 0.1 parameters mean it will take 1.0 s and 0.1 s to get to 90% decayed or released. The 0.5 is the sustain level. The note will decay to 50% volume expoentially.

Next, we have to connect the envelope to the amplitude of the sawtooth, which we do in the setup phase with the statement:

```cpp
synth.connect(envelope, "signal", saw, "amplitude");
```

And finally, we need to make sure the envelope starts over every time we begin a new note with the *player*. This can be done using the *trigger* method of the *Envelope* class:

```cpp
player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
    saw.set_input("frequency", freq);
    envelope.trigger();
}, 3.0);
```

The entire example is now

```cpp
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    // Declare
    Container synth;
    Audio audio;
    Saw saw("raw");
    Player player;
    Envelope envelope;

    // Setup
    synth.connect(saw, "signal", audio, "left")
         .connect(saw, "signal", audio, "right")
         .connect(envelope, "signal", saw, "amplitude")
         .add(player);

    player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
        saw.set_input("frequency", freq);
        envelope.trigger();
    }, 3.0);

    envelope.set_adsr(0.8, 1.0, 0.5, 0.1);

    // Execute
    synth.run(UNTIL_INTERRUPTED);   

    // Cleanup
    return 0; 

}
```

Now when you compile and run, your ears won't be nearly as offended. Although we can continue to refine this patch. 

Adding a Filter
===

To mellow out the sound of the sawtooth wave, we can filter out some of the high frequencies using a low pass filter. A standard choice is the [Biquad](classyase_1_1_biquad.html) filter, which is a second order IIR filter with an easily tunable resonance and cutoff. We declare a Biquad filter like this:

```cpp
Biquad filter;
```

By default, this filter is a low pass filter, but you can use the *Biquad::set_type* method to change it to a high pass, band pass, or all pass filter. 

Next, we insert the filter into the *synth* container after the sawtooth and before the audio module.

```cpp
synth.connect(saw,filter)
     .connect(filter, "signal", audio, "left")
     .connect(filter, "signal", audio, "right")
     .connect(envelope, "signal", saw, "amplitude")
     .add(player);
```

Note that the two argument version of the *connect* method assumes the names of the inputs and outputs are "signal". This just makes the code a bit easier to read. 

Next, we can set the resonance and cutoff frequency of the filter in the set up secction:

```cpp
filter.set_input("resonance", 10);
filter.set_input("frequency", 2000);
```

Try different resonances and frequencies for the filter. Be careful with high resonances because the filter can self-oscillate, which is hard on the ears. 

The entire program now looks like this:

```cpp
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    // Declare
    Container synth;
    Audio audio;
    Saw saw("raw");
    Player player;
    Envelope envelope;
    Biquad filter;

    // Setup
    synth.connect(saw,filter)
         .connect(filter, "signal", audio, "left")
         .connect(filter, "signal", audio, "right")
         .connect(envelope, "signal", saw, "amplitude")
         .add(player);

    player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
        saw.set_input("frequency", freq);
        envelope.trigger();
    }, 3.0);

    envelope.set_adsr(0.8, 1.0, 0.5, 0.1);

    filter.set_input("resonance", 10);
    filter.set_input("frequency", 2000);

    // Execute
    synth.run(UNTIL_INTERRUPTED);   

    // Cleanup
    return 0; 

}
```

A Filter Envelope
===

Just like an envelope can modulate a sound's amplitude over time, an envelope can also modulate a filter's parameters over time. This makes a much more interesting sound than a constant filter. For example, the "wah" sound of a wah-wah pedal comes from modulating a low pass filter's cuttoff frequency from low to high as a note is played. 

To accomplish this, let's use another envelope. So that we don't get our two filters confused, we'll change the name of our existing envelop as well.

```cpp
Envelope wave_envelope, filter_envelope;
```

We would like to connect the "signal" output of the *filter_envelope* to the "frequency" input of the filter. But the envelope's output ranges from 0 to 1, while an interesting cutoff frequency would range from something like 500 Hz to 4000 Hz. Thus, we need to transform the output of the *filter_envelope* to that range. We can do this with a [Transform](classyase_1_1_transform.html) module, which we declare as follows:

```cpp
Transform scale( [] (double u) { return 500 + 4000*u; });
```

The constructor to *Transform* takes a function, which we specify here using a C++ lambda expression. The lambda takes an argument u, which is the input to the transform, and returns a scaled and shifted value, which will be the output to the transform. We can then connect the transform, filter_envelope, and filter like this:

```cpp
synth.connect(filter_envelope, scale)
     .connect(scale, "signal", filter, "frequency");
```

Each time a new note is played, we need to re-trigger both envelopes, which we do like this:

```cpp
player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
    saw.set_input("frequency", freq);
    wave_envelope.trigger();
    filter_envelope.trigger();
}, 3.0);
```

Then we'll just set up the parameters of the filter envelope, using slightly different values than the wave envelope for a nice effect.

```cpp
filter_envelope.set_adsr(2.0, 2.0, 0.0, 0.1); 
```

Now our program looks like this:

```cpp
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    // Declare
    Container synth;
    Audio audio;
    Saw saw("raw");
    Player player;
    Envelope wave_envelope, filter_envelope;
    Transform scale( [] (double u) { return 500 + 4000*u; });
    Biquad filter;

    // Setup
    synth.connect(saw,filter)
         .connect(filter, "signal", audio, "left")
         .connect(filter, "signal", audio, "right")
         .connect(wave_envelope, "signal", saw, "amplitude")
         .connect(filter_envelope, scale)
         .connect(scale, "signal", filter, "frequency")
         .add(player);

    player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
        saw.set_input("frequency", freq);
        wave_envelope.trigger();
        filter_envelope.trigger();
    }, 3.0);

    wave_envelope.set_adsr(0.8, 1.0, 0.5, 0.1);
    filter_envelope.set_adsr(2.0, 2.0, 0.0, 0.1);  

    filter.set_input("resonance", 10);
    filter.set_input("frequency", 2000);

    // Execute
    synth.run(UNTIL_INTERRUPTED);   

    // Cleanup
    return 0; 

}
```

Adding an Echo Effect
===

Finally, we'll take our patch to an entirely new level by adding an [Echo](classyase_1_1_echo.html). This will introduce significantly more texture to the patch. One thing that will happen is that many attenuated copies of our original sound will be added to each other. The result is interesting, but also a lot louder. So we'll also want to attenuate the total sound, which we'll do by adding a volume [Gain](classyase_1_1_gain.html). We can also get a cute stereo effect by sending the output of the echo to the left channel, and an inverted copy of the output to the right channel. 

We declare these modules like this:

```cpp
Echo echo;
Gain gain;
Transform invert( [] (double u) { return -u; });
```

set up the gain like this:
```cpp
gain.set_input("amplitude", 0.35);
```

and connect them like this:

```cpp
synth.path(saw, filter, echo, gain, invert)
     .connect(gain, "signal", audio, "left")
     .connect(invert, "signal", audio, "right");
```

Then we set the parameters of the echo effect. The "duration" of the echo is an integer number of samples to store in the echo's delay buffer. For a 0.5 s delay, we use SAMPLE_RATE / 2 = 22050 samples:

```cpp
echo.set_input("duration", SAMPLE_RATE/2);
```

We also need the amount of signal to feed back into the system. Choosing 80% sounds good:

```cpp
echo.set_input("amplitude", 0.8);
```

The final program is now:

```cpp
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    // Declare
    Container synth;
    Audio audio;
    Biquad filter;
    Saw raw("raw");
    AntiAlias saw(raw);
    Envelope wave_envelope, filter_envelope;
    Player player;
    Transform scale( [] (double u) { return 500 + 4000*u; }),
              invert( [] (double u) { return -u; });
    Echo echo;
    Gain gain;      

    // Setup
    synth.path(saw, filter, gain, echo, invert)
         .connect(echo, "signal", audio, "left")
         .connect(invert, "signal", audio, "right")
         .connect(wave_envelope, "signal", saw, "amplitude")
         .connect(filter_envelope, scale)
         .connect(scale, "signal", filter, "frequency")
         .add(player);

    player.set({ G3, C4, A3, F4, F2, F1 }, [&] (double freq) {
      saw.set_input("frequency", freq);
      wave_envelope.trigger();
      filter_envelope.trigger();
    }, 3.0);

    saw.set_input("amplitude", 0.4);
    filter.set_input("resonance", 10);
    gain.set_input("amplitude", 0.35);
    echo.set_input("duration", SAMPLE_RATE/2);
    echo.set_input("amplitude", 0.8);
    wave_envelope.set_adsr(0.8, 1.0, 0.5, 0.1);
    filter_envelope.set_adsr(2.0, 2.0, 0.0, 0.1);      

    // Execute
    synth.run(UNTIL_INTERRUPTED);

    // Cleanup
    return 0; 

}
```

Compile and run this program and you will hear a pretty cool sound! 

Next Steps
===

As you can see, building a patch in Yase is fairly straightforward. You declare some modules, connect them together, set their parameters, and run. Try building on this example. Here are some ideas:
- Try changing the attack, sustain, decay, and release parameters, the duration of the player's steps, the duration of the echo, and the amount of the echo to see how these parameters affect the sound. 
- Try using a [Square](classyase_1_1_square.html) wave instead of a sawtooth wave.
- Try using a [Noise](classyase_1_1_noise.html) source instead of a sawtooth wave. 
- Tye using a high pass filter instead of a low pass filter. You might want to modify the offset and range of the cuttoff frequency in the *scale* transform to get pleasing sound.