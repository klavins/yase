#ifndef YASE_TRIANGLE_H
#define YASE_TRIANGLE_H

#include <string.h>
#include "yase.hh"

namespace yase {

    class Triangle : public Oscillator {

    typedef  void (Triangle::*UpdateFunction)(); 

    public:
      Triangle();
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
