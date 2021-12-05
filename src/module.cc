#include "exception.hh" 
#include "module.hh"

namespace yase {

    Module::Module() {

    }

    int Module::add_input(string name) {
        input_map[name] = input_map.size();
        inputs.push_back(0.0); // resizes vector
        return input_map[name];
    }

    int Module::add_output(string name) {
        output_map[name] = output_map.size();
        outputs.push_back(0.0); // resizes vector
        return output_map[name];
    }

    int Module::get_input_index(string name) const {
        if ( input_map.count(name) > 0 ) {
            return input_map.at(name); // Note: input_map[name] does not
                                       // work here because [] is not const.
                                       // in fact, it inserts an element if
                                       // one doesn't exist!
        } else {
            throw Exception("Tried to access a non-existant input " + name + " in get_input_index.");
        }
    }

    int Module::get_output_index(string name) const {
        if ( output_map.count(name) > 0 ) {
            return output_map.at(name);   
        } else {
            throw Exception("Tried to access a non-existant output " + name + " in get_output_index.");
        }
    }

    void Module::set_input(string name, double value) {
        if ( input_map.count(name) > 0 ) {
          inputs[input_map.at(name)] = value; 
        } else {
            throw Exception("Tried to access a non-existant input " + name + " in set_input.");
        }
    }

    void Module::set_input(int index, double value) {
        inputs[index] = value;
    }  

    double Module::get_output(string name) const {
        if ( output_map.count(name) > 0 ) {
          return outputs[output_map.at(name)];
        } else {
            throw Exception("Tried to access a non-existant output " + name + " in get_input.");
        }
    }

    double Module::get_output(int index) const {
        return outputs[index];
    }   

}