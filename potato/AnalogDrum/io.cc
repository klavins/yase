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

        switches = i2c_smbus_read_byte_data(file, 0x40);

        sw[0] = ( switches & 0b00000001 ) == 1;
        sw[1] = ( switches & 0b00000010 ) == 2;
        sw[2] = ( switches & 0b00000100 ) == 4;
        sw[3] = ( switches & 0b00001000 ) == 8;

        __u8 x = 0x80;

        x = x | ( led[0] ? 1 : 0 );
        x = x | ( led[1] ? 2 : 0 );
        x = x | ( led[2] ? 4 : 0 );
        x = x | ( led[3] ? 8 : 0 );

        i2c_smbus_read_byte_data(file, x);

        n = 0;

    }

  }

}
