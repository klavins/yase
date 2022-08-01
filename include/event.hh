// 
// YASE Event Header
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
