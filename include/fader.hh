// 
// YASE Fader Module Header
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

#ifndef YASE_FADER_H
#define YASE_FADER_H

#include "module.hh"

namespace yase {

    //! A Fader Module maps an input target to an output value. 
    
    //! Typically,
    //! the input comes from a Midi control such as a knob. Because such 
    //! controls are discrete (for example, integers from 0 to 127), directly
    //! mapping the input to the output would result in abropt changes to
    //! the Fader value. The Fader object solves this with an internal state
    //! that uses proportional feedback to track the target, but moves continuously. 
    //! 
    //! Typically, a list of fader objects is managed by a Controls object and a Fader
    //! object is not managed by the user directly.
    //! 
    class Fader : public Module {

    public:

      Fader();
      Fader(double min, double max);
      void init();
      void update();
      void randomize();

      //! Set the tracking gain of the fader. A higher number results in a snappier response. 
      //! \param x The desired gain
      inline void set_tracking_gain(double x) { tracking_gain = x; }

      //! Set the response of the fader to lienar.
      inline void linear() { is_linear = true; }

      //! Set the reponse of the fader to exponential with base b. See the documentation for Controls
      //! for details. 
      //! \param b The base of the exponential.
      inline void exponential(double b) { is_linear = false; base = b; }  

    private:

      double adjusted_target();    

      double min_val,
             max_val,
             tracking_gain,
             base;

      int target, value;

      bool is_linear;

    };

}

#endif
