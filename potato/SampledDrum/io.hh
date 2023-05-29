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
      inline bool switch_state(int i) { return sw[i]; }
      void set_led(int i, bool state) { led[i] = state; }

    private:

      int knob_0, knob_1, knob_2, knob_3,
          file,
          n = 0;

      bool sw[4], led[4];

      double value;

      __s32 res, switches;
      __u8 reg;

    };

}

#endif
