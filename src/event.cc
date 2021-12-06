#include <math.h>
#include "event.hh"

namespace yase {

  double Event::frequency() const {
      return (440.0 / 32) * pow(2, ((id - 9) / 12.0));
  }

}

