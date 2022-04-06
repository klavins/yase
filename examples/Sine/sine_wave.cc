#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    Sine sine1, sine2;
    Audio audio;
    Container synth;
    
    sine1.set_input("frequency", 440);
    sine2.set_input("frequency", 441);

    synth.connect(sine1,"signal",audio,"left")
         .connect(sine2,"signal",audio,"right");

    synth.run(UNTIL_INTERRUPTED);

    return 0; 

}
 