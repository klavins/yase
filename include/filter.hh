#ifndef YASE_FILTER_H
#define YASE_FILTER_H

#include "module.hh"

namespace yase {

    class Filter : public Module {

    // An IIR filter 

    public:

      Filter(int P, int Q); // size of IIR filter
      void init();
      void update();
      void set_coefficients(vector<double> A, vector<double> B);

    protected:

      int p, q;
      vector<double> u, y, a, b;
      int signal;

    };

}

#endif
