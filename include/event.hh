#ifndef YASE_EVENT_H
#define YASE_EVENT_H

#include <string.h>

namespace yase {

    //! Contains MIDI event information. 
    
    //! For example, if a key press event such as 144 65 90 was sent from port 2,
    //! then code will be 144, id will be 65, value will be 90 (i.e. the velocity), and port will be 2.
    class Event {

    public:

      Event(int code, int id, int value, int port) :
          code(code),
          id(id),
          value(value),
          port(port) {}

      Event(const Event &e) : 
              
          code(e.code),  
          id(e.id), 
          value(e.value), 
          port(e.port) {} 

      double frequency() const;
      int code,  //!< The MIDI code (first byte set) 
          id,    //!< The MIDI id (second byte sent)
          value, //!< The MIDI value (third byt sent)
          port;  //!< The MIDI port

    private:

    };

}

#endif
