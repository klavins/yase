#ifndef YASE_ENVELOPE_H
#define YASE_ENVELOPE_H

#include "yase.hh"

namespace yase {

    class Envelope;

    typedef  void (Envelope::*UpdateFunction)();  

    class Envelope : public Module {

    public:

      Envelope();
      void init();
      void update(); 
      void trigger();
      void release();

    private:

      // i/o indices
      int signal, 
          a,d,s,r, 
          velocity;

      UpdateFunction update_fcn;

      double amplitude;

      void off();
      void attack();
      void decay();
      void sustain();
      void _release();          

    };

}

#endif
