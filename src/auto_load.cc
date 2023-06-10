// 
// YASE AutoLoad Module Implementation
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

#include "auto_load.hh"
#include "yase.hh"
#include <unistd.h>

namespace yase {

  AutoLoad::AutoLoad(string path) : path(path), monitor(NULL), active(false) {

    parameters = get_config(path);

  }

  AutoLoad::~AutoLoad() {
    if ( monitor ) {
      DEBUG
      active = false;
      monitor->join();
      delete monitor;
    }
  }

  void AutoLoad::reload() {

   std::cout << "monitor thread starting\n";

   while(active) {

      try {
        parameters = get_config(path);
      } catch(...) {
        std::cout << "could not open " << path << "\n";
      }

      while ( !token.try_lock() ) {} // wait for mutex

      for ( auto& [category, params] : parameters.items() ) {
        for ( auto& [name,value] : params.items() ) {
          try {
            int i = get_output_index(category + ":" + name);
            if ( new_outputs[i] != value ) {
              new_outputs[i] = value;
            }
          } catch(yase::Exception) {}
        }
      }

      token.unlock();
      usleep(250000);

   }

  }

  bool AutoLoad::provides(string category, string param) {
    return parameters.contains(category) && parameters[category].contains(param);
  }

  void AutoLoad::associate(string category, string param, Module& module) {

    if ( !parameters[category].contains(param) ) {
      std::cout << "Warning: " << path << " does not defined " << category << "." << param << "\n";
    }
    // TODO: check for duplicate output names
    int i = add_output(category + ":" + param);
    outputs[i] = parameters[category][param];
    new_outputs.push_back( parameters[category][param]);

  }

  void AutoLoad::init() {
    std::cout << "Starting auto loader\n";
    active = true;
    monitor = new std::thread(&AutoLoad::reload, this);
  }

  void AutoLoad::update() {

    if ( token.try_lock() ) {
          for ( int i =0; i<outputs.size(); i++) {
            outputs[i] = new_outputs[i];
          }
          token.unlock();
    }

  } 

}
