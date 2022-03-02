#ifndef YASE_TRANSFORM_H
#define YASE_TRANSFORM_H

#include "yase.hh"

namespace yase {

    //! Transform an input signal with a stateless, user-defined
    //! function. For example
    //! \code
    //! Transform invert( [] (double u) { return -u; });
    //! \endcode
    //! \param[input] signal
    //! \param[output] signal
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
