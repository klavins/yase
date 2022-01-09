#include "yase.hh"

namespace yase {

  EventManager::EventManager() {

  }

  EventManager &EventManager::listen(int event_type, std::function<void(const Event &)> handler) {
    listeners[event_type].push_back(handler);
    return *this;
  }

  EventManager &EventManager::listen(int event_type, int port, function<void(const Event &)> handler) {
    listeners[event_type].push_back([port, handler] (const Event & e) {
        if ( e.port == port ) {
          handler(e);
        }
    });
    return *this; 
  }  

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

  void EventManager::respond_to(const Event &event) {
      for(auto handler : listeners[event.code]) {
        handler(event);
      }
      for(auto handler : listeners[MIDI_ANY]) {
        handler(event);
      }     
  }


}