// 
// YASE Event Manager Header
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

#ifndef YASE_EVENTMANAGER_H
#define YASE_EVENTMANAGER_H

#include <algorithm>
#include <map>
#include "module.hh"
#include "event.hh"

using std::vector;
using std::tuple;
using std::map;
using std::function;

namespace yase {

    //! A class that keeps track of event listeners. 
    
    //! This class can be used in isolation,
    //! but is usually inheritied from. For example, the Container class inherits from
    //! this class. 
    class EventManager {

    public:

      EventManager();
      void process_events(vector<Module *> &modules);
      void respond_to(const Event &event);
      EventManager &listen(int event_type, function<void(const Event &)> handler);
      EventManager &listen(int event_type, int port, function<void(const Event &)> handler);      

    protected:

      map<int,vector<function<void(const Event&)>>> listeners;    

    };

}

#endif