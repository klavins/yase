#ifndef YASE_TRANSFORM_H
#define YASE_TRANSFORM_H

#include "yase.hh"

namespace yase {

    class Transform : public Module {

    public:

      Transform(function<double(const double u)>);
      void init();
      void update();

    private:

      function<double(const double u)> f;
      int signal;

    };

}

#endif
