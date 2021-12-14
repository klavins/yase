#ifndef YASE_EVENT_H
#define YASE_EVENT_H

#include <string.h>

namespace yase {

    class Event {

    public:

      Event(int code, int id, int value, int port) :
          code(code),
          id(id),
          value(value),
          port(port) {}
      double frequency() const;
      int code, id, value, port;

    private:

    };

}

#endif
