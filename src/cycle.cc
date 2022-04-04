#include "cycle.hh"
#include "yase.hh"

namespace yase {

  Cycle::Cycle() : pitches({}) {

  }

  //! Set the list of pitches, the callback function, and the duration of each step in the cycle. 
  //! For example, 
  //! 
  //!     cycle.set({ 440, 587.33, 220, 659.26, 246.94, 293.67 }, [&] (double freq) {
  //!         osc.set_input("frequency", freq);
  //!     }, 1.0);
  //! 
  //! \param pitch_list the list of pitches
  //! \param f the callback function, which should take a pitch as an argument
  //! \param dt The duration of each step
  void Cycle::set(vector<double> pitch_list, function<void(double)> f, double dt) {
    callback = f;
    pitches = pitch_list;
    duration = dt;
  }

  void Cycle::init() {

    t = 0;
    n = 0;

    if ( pitches.size() > 0 ) {
      callback(pitches[n]);
      n++;
      n = n % pitches.size();
    }


  }

  void Cycle::update() {

    t += TS;

    if ( t >= duration ) {
      callback(pitches[n++]);
      n = n % pitches.size();      
      t = 0;
    }

  }    

}
