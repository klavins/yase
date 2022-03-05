#include <iostream>
#include "yase.hh"

using namespace yase;

int main(int argc, char * argv[]) {

    std::cout << argv[1] << "\n";

    Audio audio(2,1);
    Sampler sampler(argv[1]);
    Container container;
    
    container.add(sampler)
             .add(audio)
             .connect(audio, "line_in0", sampler, "signal");
    
    sampler.record();
    container.run(UNTIL_INTERRUPTED);

    std::cout << "done recording\n";

    sampler.stop()
           .write();

    return 0; 

}
 
