Knobs Demo
===

This demo expect's Todd's IO board to be hooked up to i2c and visible with i2cdetect. 

You can make the demo by running
```bash
make
```

and run it by doing

```bash
sudo ./bin/knobs
```

Once it is running you should hear a repeating pattern of notes that use a sawtooth wave running through a biquad low pass filter. You can use knob 0 to change the cutoff frequency of the filter and knob 1 to change the resonance frequence of the filter. 
