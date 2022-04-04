#include <iostream>

#include <math.h>
#include "yase.hh"

using namespace yase;


int main(int argc, char * argv[]) {

    double freq = 1975; // B6
    double cycles = 10;

    Saw osc;
    osc.set_type("additive");
    osc.set_input("frequency", freq);
    FFT fft(cycles*SAMPLE_RATE / freq);

    Container synth;
    synth.connect(osc,fft);
    synth.run(cycles*SAMPLE_RATE/freq+1);

    fft.html();

    return 0; 

}
 
