#ifndef YASE_FILTER_H
#define YASE_FILTER_H

#include "module.hh"

namespace yase {

    //! This module implements an Infinite Impulse Response (IIR) filter.
    
    //! The IIR operates according to the difference
    //! equation
    //!
    //!       a[0]*y[n] + a[1]*y[n-1] + ... + a[p]*y[n-p] = b[0]*u[n] + b[1]*u[n-1] + ... + b[q]*u[n-q].
    //!
    //! The input is u[n] and the output is y[n]. Previous values of u and y are remembered by the module. 
    //! There are two constructors. One takes integers p and q, specifying the feedforward and feedback orders 
    //! respectively. A later call to 
    //! set_coefficients is needed in this case. The other constructor takes a list of the coefficients
    //! directly. 
    //!
    //! For example, a simple averaging filter (which is actually a FIR filter) can be constructed with
    //! 
    //!       IIRFilter filter({2},{1,1});
    //!
    //! \param[in] signal
    //! \param[out] signal
    class IIRFilter : public Module {

    public:

      IIRFilter(int P, int Q); // size of IIR filter
      IIRFilter(vector<double> A, vector<double> B); // coeefficient list
      void init();
      void update();
      void set_coefficients(vector<double> A, vector<double> B);

    protected:

      int p, q;
      vector<double> u, y, a, b;
      int signal, offset;

    };

}

#endif
