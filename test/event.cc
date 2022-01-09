#include <typeinfo>
#include <iostream>

#include "yase.hh"
#include "gtest/gtest.h"

namespace {

    using namespace yase;

    class MyModule : public Module {
        public:
        void init() {
            sent = false;
        }
        void update() {
            if ( ! sent ) {
                emit(Event(1,2,3,4));
                sent = true;
            }
        }
        bool sent;

    };

    TEST(EVENT,IO) {
        MyModule mm;
        Synthesizer synth("MidiMix");
        int x = 0;
        synth.add(mm)
             .listen(1, [&x] (const Event &e) {
                 std::cout << e.code << std::endl;
                 x += e.code;
             });
        synth.run(10);
        ASSERT_EQ(x,1);
    }

    
}