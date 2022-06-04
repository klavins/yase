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