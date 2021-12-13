#ifndef YASE_SAW_H
#define YASE_SAW_H

#include <string.h>
#include "yase.hh"

namespace yase {

    class Saw : public Oscillator {

    typedef void (Saw::*UpdateFunction)(); 

    public:

      void init();
      void update();
      void set_type(std::string name);

    private:

      UpdateFunction update_fcn;

      void raw();
      void ptr1();
      void additive();

    };

}

#endif
