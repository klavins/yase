// 
// YASE Buffer Module Header
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

#ifndef YASE_BUFFER_H
#define YASE_BUFFER_H

#include "yase.hh"
#include "ring_buffer.hh"

namespace yase {

    class Buffer : public Module {

    public:

      typedef tuple<long int, double> ElementType;

      Buffer(int size);
      void init();
      void update();
      int get_buffered_output(ElementType * result, int max);

    private:

      int size, signal;
      unsigned long int count;
      RingBuffer<ElementType> ring_buffer;
      tuple<long int, double> data;

    };

}

#endif
