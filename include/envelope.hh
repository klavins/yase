// 
// YASE Echo Module Header
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

#ifndef YASE_ENVELOPE_H
#define YASE_ENVELOPE_H

#include "yase.hh"

namespace yase {

    class Envelope;

    typedef  void (Envelope::*UpdateFunction)();  

    //! An ADSR envelope
    
    //! Typically, the input signal is not used. The output is fed to, 
    //! for example, the amplitude of a gain or the frequency of a filter.
    //! The attack phase is linear, the decay and release phases are exponential.
    //! The attack input determines the length of the attack in seconds. The
    //! release and decay inputs determine the release and decay rates according
    //! to 
    //!
    //! \f$y = e^{-t/k}\f$
    //! 
    //! where k is either the (decay or release) parameter.
    //! \param[in] attack
    //! \param[in] decay
    //! \param[in] sustain    
    //! \param[in] release
    //! \param[in] signal
    //! \param[out] signal
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
