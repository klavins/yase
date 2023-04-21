// 
// YASE Event Manager Implementation
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

#include "yase.hh"

namespace yase {

  //! Make a new event manager.
  EventManager::EventManager() {

  }

  //! Listen for a MIDI event and respond. An event is emited by some other Module using the emit() method.
  //! \param event_type The type of event. For example, MIDI_KEYDOWN, MIDI_KEYUP, or MIDI_ALL
  //! \param handler A method to run when the event is processed.
  EventManager &EventManager::listen(int event_type, std::function<void(const Event &)> handler) {
    listeners[event_type].push_back(handler);
    return *this;
  }

  //! Listen for a MIDI event and respond. An event is emited by some other Module using the emit() method.
  //! \param event_type The type of event. For example, MIDI_KEYDOWN, MIDI_KEYUP, or MIDI_ALL
  //! \param port The MIDI port to listen to. 
  //! \param handler A method to run when the event is processed.
  EventManager &EventManager::listen(int event_type, int port, function<void(const Event &)> handler) {
    listeners[event_type].push_back([port, handler] (const Event & e) {
        if ( e.port == port ) {
          handler(e);
        }
    });
    return *this; 
  }

  //! Process all events that have been emitted by the provided list of Modules, and deactiviating them.
  //! \param modules A vector of Modules
  void EventManager::process_events(vector<Module *> &modules) {
    for(Module * m : modules ) {
      for(Event &event : m->events) {
          for(auto handler : listeners[event.code]) {
            handler(event);
          }
          for(auto handler : listeners[MIDI_ANY]) {
            handler(event);
          }          
      }
      m->events.clear();
    }
  }

  //! Run any event handlers (set up by a call to "listen") for the given Event
  //! \param event The event to respond to
  void EventManager::respond_to(const Event &event) {
      for(auto handler : listeners[event.code]) {
        handler(event);
      }
      for(auto handler : listeners[MIDI_ANY]) {
        handler(event);
      }     
  }


}