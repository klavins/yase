// 
// YASE Module Header
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

#ifndef YASE_MODULE_H
#define YASE_MODULE_H

#include <string>
#include <map>
#include <vector>

#include "event.hh"

namespace yase {

    using std::string;
    using std::map;
    using std::vector;

     //! An abstract base class for modules.

     //! Derived classes should imlement the init and update methods.
    class Module {

    friend class Synthesizer;
    friend class EventManager;

    public:

    Module();

    //! This method should be overridden by derived
    //! classes. It will usually be called once, after all modules and
    //! connections objects have been added to a synthesizer, but before
    //! the synthesizer starts running.
    virtual void init() = 0;

    //! This method should be  overridden by derived
    //! classes. It will be called repeatedly by a synthesizer at a frequency
    //! determined by SAMPLE_RATE.    
    virtual void update() = 0;

    int add_input(string name); 
    int add_output(string name); // adds a new output
    int get_input_index(string name) const;  
    int get_output_index(string name) const; // get the output index corresponding to the name
    void set_input(string name, double value); 
    void set_input(int index, double value);
    double get_input(int index) const; 
    double get_input(string name) const;
    double get_output(string name) const; // simular to inputs
    double get_output(int index) const;
    void set_output(int index, double value);
    void copy_inputs(const Module &source);
    void copy_outputs(Module &destination) const;
    void emit(Event e);

    inline void set_ts(double s) { ts = s; }
    inline double get_ts() { return ts; }

    protected:

    map<string,int> input_map,  // relates input names to their indices
                    output_map; // relates output names to their indices

    vector<double> inputs,   // values of the inputs
                   outputs;  // values of the outputs

    vector<Event> events;

    double ts;

    };

}

#endif