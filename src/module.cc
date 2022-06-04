// 
// YASE Base Module Implementation
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

#include <iostream>
#include "yase.hh" 

namespace yase {

    Module::Module() : ts(TS) {}

    //! This method is used to add a new input to the module. The name 
    //! can be later used to refer to this input. This method is typically called
    //! in the constructor of derived classes.
    //! \param name The name of the input
    //! \return The index of the input, which can also be used to refer to this input.
    int Module::add_input(string name) {
        input_map[name] = input_map.size();
        inputs.push_back(0.0); // resizes vector
        return input_map[name];
    }

    //! This method is used to add a new output to the module. The name 
    //! can be later used to refer to this input. This method is typically called
    //! in the constructor of derived classes.
    //! \param name The name of the output
    //! \return The index of the output, which can also be used to refer to this input.
    int Module::add_output(string name) {
        output_map[name] = output_map.size();
        outputs.push_back(0.0); // resizes vector
        return output_map[name];
    }

    //! Get the index corresponding to the name used when the input added using add_input. 
    //! The index is should be used in the update method of a module rather than the
    //! name because it is faster.    
    //! \param The name of the output
    //! \return The index of the output
    int Module::get_input_index(string name) const {
        if ( input_map.count(name) > 0 ) {
            return input_map.at(name); // Note: input_map[name] does not
                                       // work here because [] is not const.
                                       // in fact, it inserts an element if
                                       // one doesn't exist!
        } else {
            throw Exception("Tried to access a non-existant input '" + name + "' in get_input_index.");
        }
    }

    //! Get the index corresponding to the name used when the output added using add_output. 
    //! The index is should be used in the update method of a module rather than the
    //! name because it is faster.    
    //! \param The name of the input
    //! \return The index of the input
    int Module::get_output_index(string name) const {
        if ( output_map.count(name) > 0 ) {
            return output_map.at(name);   
        } else {
            throw Exception("Tried to access a non-existant output '" + name + "' in get_output_index.");
        }
    }

    //! Set the value of the input refered to by name. This version of the set method
    //! be used during setup, with the name as a convenience. 
    //! \param name The name of the input
    //! \param value The value to set the input to
    void Module::set_input(string name, double value) {
        if ( input_map.count(name) > 0 ) {
          inputs[input_map.at(name)] = value; 
        } else {
           throw Exception("Tried to access a non-existant input '" + name + "' in set_input.");
        }
    }

    //! Set the value of the input refered to the index parameter. This version of the set method
    //! is prefered in update functions because it is faster than usig the string name. 
    //! \param name The name of the input
    //! \param value The value to set the input to    
    void Module::set_input(int index, double value) {
        inputs[index] = value;
    }  

    //! Get the value of the output refered to the index parameter. Slower than using the index
    //! of the output.
    //! \param name The name of the input   
    //! \return The valueof the output
    double Module::get_output(string name) const {
        if ( output_map.count(name) > 0 ) {
          return outputs[output_map.at(name)];
        } else {
            throw Exception("Tried to access a non-existant output '" + name + "' in get_input.");
        }
    }

    //! Get the value of the output refered to the index parameter. This version of the get method
    //! is prefered in update functions because it is faster than using the string name. 
    //! \param name The name of the input  
    //! \return The value of the output  
    double Module::get_output(int index) const {
        return outputs[index];
    }   

    //! Get the value of the input refered to the index parameter. This version of the get method
    //! is prefered in update functions because it is faster than using the string name. 
    //! \param name The index of the input
    //! \return The value of the input
    double Module::get_input(int index) const {
        return inputs[index];
    }

    //! Get the index corresponding to the name used when the input added using add_input. 
    //! The index is should be used in the update method of a module rather than the
    //! name because it is faster.
    //! \param The name of the input
    //! \return The index of the input
    double Module::get_input(string name) const {
        return inputs[get_input_index(name)];
    }    

    //! Copy the inputs of the source module to the inputs of this module. The modules
    //! should have the same number of inputs. This can be useful for modules that
    //! contain other modules. 
    //! \param The source module
    void Module::copy_inputs(const Module &source) {
        for(int i=0; i<inputs.size(); i++ ) {
            set_input(i, source.get_input(i));
        }
    }

    //! Set the value of the output refered to by the index. 
    //! \param index The index of the output
    //! \param The value to set the output to
    void Module::set_output(int index, double value) {
        outputs[index] = value;
    }

    //! Copy the outputs of this module to the destination module. The modules should
    //! have the same number of outputs. This can be useful for modules that
    //! contain other modules. 
    //! \param The destination module
    void Module::copy_outputs(Module &destination) const {
        for(int i=0; i<outputs.size(); i++ ) {
            destination.set_output(i, outputs[i]);
        }
    }    

    //! Emit an event. A containing synthesizer can listen for such events and respond to them.
    //! \param e The event to emit
    void Module::emit(Event e) {
        events.push_back(e);
    }

}