#include <iostream>

#include <math.h>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    double freq = 1975; // B6
    double cycles = 10;

    Saw osc;
    AntiAlias aa_osc(osc);
    osc.set_type("raw");
    osc.set_input("frequency", freq);
    FFT fft(cycles*SAMPLE_RATE / freq);

    Container synth;
    synth.connect(aa_osc,fft);
    synth.run(cycles*SAMPLE_RATE/freq+1);

    fft.html();

    return 0; 

}