#include <math.h>
#include "midi_event.hh"

namespace yase {

  double MidiEvent::frequency() {
      return (440.0 / 32) * pow(2, ((id - 9) / 12.0));
  }

}

