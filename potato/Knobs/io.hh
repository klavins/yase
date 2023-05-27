#ifndef YASE_IO_H
#define YASE_IO_H

#include "module.hh"

#include <fcntl.h>
#include <sys/ioctl.h>
extern "C" {
  #include <linux/i2c-dev.h>
  #include <i2c/smbus.h>
}
#include <unistd.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace yase {

    class IO : public Module {

    public:

      IO();
      void init();
      void update();

    private:

      int knob_0, knob_1, knob_2, knob_3,
          file,
          n = 0;

      double value;

      __s32 res;
      __u8 reg;

    };

}

#endif
