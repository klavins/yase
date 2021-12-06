#include <typeinfo>
#include <iostream>

#include "yase.hh"
#include "gtest/gtest.h"

namespace {

    using namespace yase;

    class MyEvent : public Event {

        public:
        MyEvent(int num) : Event("my_event"), data(num) {} 
        int data;

    };

    class MyModule : public Module {
        public:
        void init() {
            sent = false;
        }
        void update() {
            if ( ! sent ) {
                emit(new MyEvent(1));
                sent = true;
            }
        }
        bool sent;

    };

    TEST(EVENT,TYPE) {
        MyEvent e(1);
        ASSERT_EQ(typeid(MyEvent), typeid(e));
    }

    TEST(EVENT,IO) {
        MyModule mm;
        Synthesizer synth;
        int x = 0;
        synth.add(mm)
             .listen("my_event", [&x] (Event * event) {
                 DEBUG
                 MyEvent * me = (MyEvent *) event;
                 std::cout << me->data << std::endl;
                 x += me->data;
             });
        synth.run(10);
        ASSERT_EQ(x,1);
    }

    
}