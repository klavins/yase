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

    class Module {

    friend class Synthesizer;

    public:

    Module();
    virtual void init() = 0;
    virtual void update() = 0;

    int add_input(string name); // adds a new input to the object, 
                                // should be called in the 
                                // constructor. 

    int add_output(string name); // adds a new output

    int get_input_index(string name) const;  // get the input index corresponding to the name
    int get_output_index(string name) const; // get the output index corresponding to the name

    void set_input(string name, double value); // set the value of the input by name
    void set_input(int index, double value);   // set the value of an input by index

    double get_output(string name) const; // simular to inputs
    double get_output(int index) const;

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