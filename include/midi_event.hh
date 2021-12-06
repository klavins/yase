#ifndef YASE_MIDIEVENT_H
#define YASE_MIDIEVENT_H

#include "yase.hh"

namespace yase {

    class MidiEvent : public Event {

    public:

      MidiEvent(string name, int code, int id, int value) 
        : Event(name),
          code(code),
          id(id),
          value(value) {}
      double frequency();
      int code, id, value;

    private:

    };

}

#endif
