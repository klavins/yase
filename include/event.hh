#ifndef YASE_EVENT_H
#define YASE_EVENT_H

namespace yase {

    class Event {

    public:

      Event(int code, int id, int value) :
          code(code),
          id(id),
          value(value) {}
      double frequency() const;
      int code, id, value;

    private:

    };

}

#endif
