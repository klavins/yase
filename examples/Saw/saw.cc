#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Saw raw, _aliased, additive;
    AntiAlias aliased(_aliased);

    raw.set_type("raw");
    _aliased.set_type("aliased");
    additive.set_type("additive");

    Audio audio;
    Container synth; 

    double freq = 1000.0;
    raw.set_input("frequency", freq);
    _aliased.set_input("frequency", freq);
    additive.set_input("frequency", freq);

    synth.connect(raw,"signal",audio,"left")
        .connect(raw,"signal",audio,"right");

    std::cout << "raw\n";
    synth.run(SAMPLE_RATE);    

    for ( int i=0; i<3; i++ ) {

        synth.disconnect(raw,"signal",audio,"left")
            .disconnect(raw,"signal",audio,"right")
            .connect(aliased,"signal",audio,"left")
            .connect(aliased,"signal",audio,"right");

        std::cout << "anti-aliased\n";
        synth.run_again(SAMPLE_RATE);

        synth.disconnect(aliased,"signal",audio,"left")
             .disconnect(aliased,"signal",audio,"right")
             .connect(additive,"signal",audio,"left")
             .connect(additive,"signal",audio,"right");

        std::cout << "additive\n";
        synth.run_again(SAMPLE_RATE);

        synth.disconnect(additive,"signal",audio,"left")
             .disconnect(additive,"signal",audio,"right")  
             .connect(raw,"signal",audio,"left")
             .connect(raw,"signal",audio,"right");

        std::cout << "raw\n";
        synth.run_again(SAMPLE_RATE);                  

    }

    return 0; 

}
 
