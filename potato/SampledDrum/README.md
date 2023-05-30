Analog Drum Demo
===

This demo expect's Todd's IO board to be hooked up to i2c and visible with i2cdetect. 

You will also need a third party library for reading audio files:

```bash
cd yase/potato
mkdir third-party
cd third-party
git clone https://github.com/adamstark/AudioFile.git
cd ..
make -j4
```

Then you can make the demo by running
```bash
cd SampledDrum
make -j4
```

and run it by doing

```bash
sudo ./bin/sampled_drum
```

Once running you should hear a kick, snare, bass and lead. The switches turn these tracks on and off and the knobs 
adjust the cutoff, resonance, attack and sustain of the bass. The leds should tun on and off approximately in sync with the tracks. 

