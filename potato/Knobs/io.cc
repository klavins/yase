#include "io.hh"
#include "yase.hh"

#include <fcntl.h>
extern "C" {
  #include <sys/ioctl.h>
  #include <linux/i2c-dev.h>
}
#include <i2c/smbus.h>
#include <unistd.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace yase {

  IO::IO() {

    knob_0 = add_output("knob_0");
    knob_1 = add_output("knob_1");
    knob_2 = add_output("knob_2");
    knob_3 = add_output("knob_3");

    file = open("/dev/i2c-1", O_RDWR);
    assert(file >= 0);

    int addr = 0x16;
    reg = 0x01;
    assert (ioctl(file, I2C_SLAVE, addr) >= 0);

    n = 0;

  }

  void IO::init() { }

  void IO::update() {

    if ( n++ > SAMPLE_RATE / 10 ) {

        outputs[knob_0] = i2c_smbus_read_byte_data(file, 0x01) / 255.0;
        outputs[knob_1] = i2c_smbus_read_byte_data(file, 0x11) / 255.0;
        outputs[knob_2] = i2c_smbus_read_byte_data(file, 0x21) / 255.0;
        outputs[knob_3] = i2c_smbus_read_byte_data(file, 0x31) / 255.0;

        n = 0;

    }

  }

}
