#include "module.hh"
#include "iir_filter.hh"
#include <iostream>

namespace yase {

  IIRFilter::IIRFilter(int P, int Q) : p(P), q(Q) {

    for(int i=0; i<p; i++) {
      y.push_back(0);
      a.push_back(0);
    }

    for(int i=0; i<q; i++) {
      u.push_back(0);
      b.push_back(0);
    }    

    signal = add_input("signal");
    signal = add_output("signal"); 
    offset = add_input("offset");

    set_input(offset, 0.0);

  }

  IIRFilter::IIRFilter(vector<double> A, vector<double> B) : IIRFilter(A.size(), B.size()) {
    set_coefficients(A,B);
  }

  void IIRFilter::init() {

  }

  void IIRFilter::update() {


    double s1 = 0, s2 = 0;
    int i;

    for ( i=q-1; i >= 1; i-- ) u[i] = u[i-1];
    u[0] = inputs[signal];

    for ( i=p-1; i >= 1; i-- ) y[i] = y[i-1];

    for ( i=0; i<q; i++) s1 += b[i] * u[i];
    for ( i=1; i<p; i++) s2 += a[i] * y[i];    

    y[0] = (s1-s2)/a[0];

    outputs[signal] = y[0];

  }    

  void IIRFilter::set_coefficients(vector<double> A, vector<double> B) {
    a = A;
    b = B;
  }

}

