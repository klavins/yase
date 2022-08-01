// 
// YASE IIR Filter Module Implementation
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

#include "module.hh"
#include "iir_filter.hh"
#include <iostream>

namespace yase {

  //! Define a new IIR filter with feedforward order P and feedback order Q. 
  //! Call IIRFilter::set_coefficients to set the value of the coefficents.
  //! \param P Feedforward order
  //! \param Q Feedback order. 
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

  //! Define a new IIR filter feedforward coefficients A and feedback coefficients B.
  //! \param A Feedforward coefficients.
  //! \param B Feedback coefficients. 
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

  //! Set the coefficients of the filter. 
  //! \param A Feedforward coefficients.
  //! \param B Feedback coefficients.   
  void IIRFilter::set_coefficients(vector<double> A, vector<double> B) {
    a = A;
    b = B;
  }

}

