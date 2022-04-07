#ifndef YASE_IMPULSE_H
#define YASE_IMPULSE_H

#include "yase.hh"

namespace yase {

    class Impulse : public Module {

    typedef void (Impulse::*UpdateFunction)(); 

    public:

      Impulse();
      void trigger();
      void set(int k);
      void init();
      void update();
      void set_type(std::string);

    private:

      int length, n, signal; 
      UpdateFunction update_fcn;   
      void square();
      void random();

    };

}

#endif
