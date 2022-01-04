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

     //! An abstract base class for modules
     //! Derived classes should imlement the init and update methods.
    class Module {

    friend class Synthesizer;

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

    //! This method is used to add a new input to the module. The name 
    //! can be later used to refer to this input. This method is typically called
    //! in the constructor of derived classes.
    //! \param name The name of the input
    //! \return The index of the input, which can also be used to refer to this input.
    int add_input(string name); 

    //! This method is used to add a new output to the module. The name 
    //! can be later used to refer to this input. This method is typically called
    //! in the constructor of derived classes.
    //! \param name The name of the output
    //! \return The index of the output, which can also be used to refer to this input.
    int add_output(string name); // adds a new output

    //! Get the index corresponding to the name used when the input added using add_input. 
    //! The index is should be used in the update method of a module rather than the
    //! name because it is faster.
    //! \param The name of the input
    //! \return The index of the input
    int get_input_index(string name) const;  

    //! Get the index corresponding to the name used when the output added using add_output. 
    //! The index is should be used in the update method of a module rather than the
    //! name because it is faster.    
    //! \param The name of the input
    //! \return The index of the input
    int get_output_index(string name) const; // get the output index corresponding to the name

    //! Set the value of the input refered to by name. This version of the set method
    //! be used during setup, with the name as a convenience. 
    //! \param name The name of the input
    //! \param value The value to set the input to
    void set_input(string name, double value); 

    //! Set the value of the input refered to the index parameter. This version of the set method
    //! is prefered in update functions because it is faster than usig the string name. 
    //! \param name The name of the input
    //! \param value The value to set the input to    
    void set_input(int index, double value);

    //! Get the value of the input refered to the index parameter. This version of the get method
    //! is prefered in update functions because it is faster than using the string name. 
    //! \param name The name of the input
    //! \return The value of the input
    double get_input(int index) const;

    //! Get the value of the input refered to the index parameter. Slower than using the index
    //! of the input.
    //! \param name The name of the input 
    //! \return The value of the input   
    double get_input(string name) const;

    //! Get the value of the output refered to the index parameter. Slower than using the index
    //! of the output.
    //! \param name The name of the input   
    //! \return The valueof the output
    double get_output(string name) const; // simular to inputs

    //! Get the value of the output refered to the index parameter. This version of the get method
    //! is prefered in update functions because it is faster than using the string name. 
    //! \param name The name of the input  
    //! \return The value of the output  
    double get_output(int index) const;

    //! Set the value of the output refered to by the index. 
    //! \param index The index of the output
    //! \param The value to set the output to
    void set_output(int index, double value);

    //! Copy the inputs of the source module to the inputs of this module. The modules
    //! should have the same number of inputs. This can be useful for modules that
    //! contain other modules. 
    //! \param The source module
    void copy_inputs(const Module &source);

    //! Copy the outputs of this module to the destination module. The modules should
    //! have the same number of outputs. This can be useful for modules that
    //! contain other modules. 
    //! \param The destination module
    void copy_outputs(Module &destination) const;

    //! Emit an event. A containing synthesizer can listen for such events and respond to them.
    //! \param e The event to emit
    void emit(Event e);

    protected:

    map<string,int> input_map,  // relates input names to their indices
                    output_map; // relates output names to their indices

    vector<double> inputs,   // values of the inputs
                   outputs;  // values of the outputs

    vector<Event> events;

    };

}

#endif