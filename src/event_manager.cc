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