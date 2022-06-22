// 
// YASE Cycle Module Implementation
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

#include "player.hh"
#include "yase.hh"

namespace yase {

  Player::Player() : pitches({}) {

  }

  //! Set the list of pitches, the callback function, and the duration of each step in the player's cycle. 
  //! For example, 
  //! 
  //!     player.set({ 440, 587.33, 220, 659.26, 246.94, 293.67 }, [&] (double freq) {
  //!         osc.set_input("frequency", freq);
  //!     }, 1.0);
  //! 
  //! \param pitch_list the list of pitches
  //! \param f the callback function, which should take a pitch as an argument
  //! \param dt The duration of each step
  void Player::set(vector<double> pitch_list, function<void(double)> f, double dt) {
    callback = f;
    pitches = pitch_list;
    duration = dt;
  }

  void Player::init() {

    t = 0;
    n = 0;

    if ( pitches.size() > 0 ) {
      callback(pitches[n]);
      n++;
      n = n % pitches.size();
    }


  }

  void Player::update() {

    t += ts;

    if ( t >= duration ) {
      callback(pitches[n++]);
      n = n % pitches.size();      
      t = 0;
    }

  }    

}
