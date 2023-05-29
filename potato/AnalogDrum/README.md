Analog Drum Demo
===

This demo expect's Todd's IO board to be hooked up to i2c and visible with i2cdetect. 

This demo seems to be more than Le Potate can handle. To get it work work (sort of) I edited `yase/include/globals.hh` and 
changed `SAMPLE_RATE` to 18000 (which is really low for audio). Then I did `make` in the potato directory.

Anyway, you can make the demo by running
```bash
make
```

and run it by doing

```bash
sudo ./bin/analog_drum
```

Once running you should hear a kick, snare, bass and lead. THe switches turn these tracks on and off and the knobs 
adjust a few parameters. 

Lots of work to do on this demo. The low sample rate doesn't help. And the choice of parameters for the knobs is not thrilling.
