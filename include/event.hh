#ifndef YASE_EVENT_H
#define YASE_EVENT_H

#include <string>

namespace yase {

    using std::string;

    class Event {

    public:

      Event(string event_name) : name(event_name) {}
      string name;

    };

}

#endif
