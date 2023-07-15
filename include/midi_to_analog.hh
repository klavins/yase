// 
// YASE MidiToAnalog Module Header
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

#ifndef YASE_MIDITOANALOG_H
#define YASE_MIDITOANALOG_H

#include "yase.hh"

namespace yase {

    class MidiToAnalog : public Container {

    public:

      MidiToAnalog(string device_name);
      void init();
      void update();

    private:

      MidiInput midi_input;
      int frequency, trigger, velocity, aftertouch;

    };

}

#endif
