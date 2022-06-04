#ifndef YASE_CONTROLS_H
#define YASE_CONTROLS_H

#include "yase.hh"
#include "json.hpp"

namespace yase {

    //! Use a Controls object to connect MIDI knobs and faders to Module inputs. 
    
    //! For example, to add a volume knob to your project, you can do 
    //! 
    //! \code
    //! Container synth;
    //! Gain gain;
    //! Controls controls;
    //! MidiInput midi("Your Device");
    //! 
    //! synth.add(gain)
    //!      .add(conrols)
    //!      .add(midi)
    //!      .propagate_to(controls); // Important!
    //!
    //!           // module, input name,  min, max, midi id                  
    //! controls.map(gain,   "amplitude", 0,   1,   123);
    //! \endcode
    //! 
    //! Note that a controls object is a Container, just like the synth Container above. Thus,
    //! it can receive events from the MidiInput object. However, you must explicitly tell the
    //! synth container to propagate events to the controls container. 
    //! 
    //! Using the example above, you can now turn the knob on your controller with MIDI id
    //! 123 and modulate the amlitude of the gain module. The controls object maps the MIDI
    //! value, which ranges from 0 to 127, to the interval [0,1]. By default, this mapping
    //! is linear. But you can make it exponential if you want more sensititvy at the low end. 
    //! For example, 
    //!
    //! \code
    //! controls.map(gain, "amplitude", 0, 1, 123)
    //!         .exponential(1e4);
    //! \endcode
    //!
    //! Here, the mapping between the MIDI knob position and the amplitude will follow
    //! 
    //! \f$\frac{(y-x)b^{m/127-1}}{b-1}+x\f$
    //! 
    //! where x and y are the minimum and maximum value of the control, b is the base of the
    //! exponential (1e4 in this example), and m is the MIDI value. 
    //! 
    //! This is easy to see graphically. In the image below, the control output ranges from 2.0 to 5.0
    //! and the response to the MIDI input is shown for various bases. 
    //! 
    //! \image html exponential-controls.png
    //! 
    //! ## Example
    //! 
    //! A complete example is listed below using a custom module to display the value of each control
    //! in real time as the associated knob is adjusted. 
    //! 
    //! \include Controls/controls.cc

    class Controls : public Container {

    public:

      Controls();
      ~Controls();
      void init();
      void update();

      Controls &map(Module &fader, int midi_id);
      Controls &map(Module &module, string name, double min, double max, int midi_id);
      Controls &map(Module &module, int index, double min, double max, int midi_id);
      Controls &map(Module &module, string name, json spec) ;

      Controls &set_tracking_gain(double x);
      Controls &linear();
      Controls &exponential(double base);

      void randomize(); 

    private:

      std::vector<Fader *> faders;

    };

}

#endif
