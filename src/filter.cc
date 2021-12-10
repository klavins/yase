#include "module.hh"
#include "filter.hh"


namespace yase {

  Filter::Filter(int P, int Q) : p(P), q(Q) {

    for(int i=0; i<p; i++) {
      u.push_back(0);
      a.push_back(0);
    }

    for(int i=0; i<p; i++) {
      y.push_back(0);
      b.push_back(0);
    }

    signal = add_input("signal");
    signal = add_output("signal");

  }

  void Filter::init() {

  }

  void Filter::update() {

    double s1 = 0, s2 = 0;
    int i;

    for ( i=p-1; i >= 1; i-- ) u[i] = u[i-1];
    for ( i=q-1; i >= 1; i-- ) y[i] = y[i-1];
    for ( i=0; i<p; i++) s1 += b[i] * u[i];
    for ( i=1; i<q; i++) s2 += a[i] * y[i];

    u[0] = inputs[signal];
    y[0] = (s1-s2)/a[0];

    outputs[signal] = y[0];

  }    

  void Filter::set_coefficients(vector<double> A, vector<double> B) {
    a = A;
    b = B;
  }

}

