#include "module.hh"
#include "exception.hh"
#include "synthesizer.hh"
#include "gtest/gtest.h"

namespace {

    using namespace yase;

    class PassThrough : public Module {

        public:

        PassThrough() {
            signal_in = add_input("signal_in");
            signal_out = add_output("signal_out");
        }

        void init() {}

        void update() {
            outputs[signal_out] = inputs[signal_in];
        }

        private:
          int signal_in, signal_out;
    };

    TEST(PASSTHROUGH,IO) {
        PassThrough pt;
        int si = pt.get_input_index("signal_in"),
            so = pt.get_output_index("signal_out");
        pt.set_input(si,3.14);
        pt.update();
        ASSERT_FLOAT_EQ(pt.get_output(so),3.14);
    }

    TEST(PASSTHROUGH,IO_NAME_ERROR) {
        PassThrough pt;
        EXPECT_THROW(
          int si = pt.get_input_index("oops"),
          Exception
        );
    }    

    TEST(PASSTHROUGH,CONNECTIONS) {
        PassThrough a, b;
        Synthesizer synth("Midi Mix");
        synth.add(a)
             .add(b)
             .connect(a, "signal_out", b, "signal_in");

        int si = a.get_input_index("signal_in"),
            so = b.get_output_index("signal_out");

        a.set_input(si, 3.14);
        synth.run(2);
        ASSERT_FLOAT_EQ(b.get_output(so),3.14);
    }

}