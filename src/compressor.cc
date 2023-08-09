// 
// YASE Compressor Module Implementation
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

// Note: This code implements the ideas in
// https://www.eecs.qmul.ac.uk/~josh/documents/2012/GiannoulisMassbergReiss-dynamicrangecompression-JAES2012.pdf


#include "compressor.hh"
#include "yase.hh"

namespace yase {

  Compressor::Compressor() {

    signal = add_output("signal");

    signal = add_input("signal");
    threshold = add_input("threshold");
    ratio = add_input("ratio");
    knee = add_input("knee");
    attack = add_input("attack");
    decay = add_input("decay");
    boost = add_input("boost");

  }

  void Compressor::init() {

    yL = 0;

  }

  void Compressor::update() {

      double T = inputs[threshold],
             W = inputs[knee],
             R = inputs[ratio],
             a = inputs[attack],
             b = inputs[decay],
             k = inputs[boost];

      xG = 20 * log10(fabs(inputs[signal])+0.01);

      if ( 2 * ( xG - T) < -W ) {
        yG = xG;
      } else if ( 2 * fabs(xG - T) <= W ) {
        yG = xG + (1/R-1) * (xG-T+W/2) * (xG-T+W/2) / (2*W);
      } else {
        yG = T + (xG -T) / R;
      }

      xL = xG - yG;

      if ( xL > yL ) {
        yL = a* yL + (1-a)*xL;
      } else {
        yL = b * yL;
      }

      c = pow(10,-yL/20);

      outputs[signal] = k * c * inputs[signal];

  } 

}
