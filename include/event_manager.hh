#ifndef YASE_EVENTMANAGER_H
#define YASE_EVENTMANAGER_H

#include <algorithm>
#include <map>
#include "event.hh"

using std::vector;
using std::tuple;
using std::map;
using std::function;

namespace yase {

    class EventManager {

    public:

      EventManager();
      EventManager &listen(int event_type, function<void(const Event &)> handler);
      EventManager &listen(int event_type, int port, function<void(const Event &)> handler);      

    protected:

      map<int,vector<function<void(const Event&)>>> listeners;    

    };

}

#endif