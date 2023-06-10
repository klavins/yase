// 
// YASE AutoLoad Module Header
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

#ifndef YASE_AUTOLOAD_H
#define YASE_AUTOLOAD_H

#include <thread>
#include "module.hh"
#include "util.hh"

namespace yase {

    //! A parameter auto-loader that continually reloads a *.json file.
    
    //! Here is an example:
    //! \include AutoLoad/auto.cc
    //! The file parameters.json contains
    //! \include AutoLoad/parameters.json
    class AutoLoad : public Module {

    public:

      AutoLoad(string path);
      ~AutoLoad();
      void init();
      void update();
      void reload();
      void associate(string category, string param, Module& module);
      bool provides(string category, string param);

    private:

      json parameters;
      string path;
      vector<double> new_outputs;
      std::mutex token;
      std::thread * monitor;
      bool active;

    };

}

#endif
