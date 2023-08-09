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

    template<int n> class Buffer : public Module {

    public:

      typedef tuple<long int, array<double, n>> ElementType;

      Buffer(int size) : ring_buffer(size) {

        for ( int i=0; i<n; i++ ) {
          add_input("signal_" + std::to_string(i));
        }

      }

      Buffer(int size, std::array<string,3> input_names) : ring_buffer(size) {

        for ( int i=0; i<n; i++ ) {
          add_input(input_names[i]);
        }

      }      

      void init() {
        count = 0;
      }

      void update() {
        std::get<0>(data) = count++;
        for ( int i=0; i<n; i++ ) {
            std::get<1>(data)[i] = inputs[i];
        }
        ring_buffer.write(&data, 1);        
      }

      int get_buffered_output(ElementType * result, int max) {
        return ring_buffer.read(result, max);
      }

    private:

      int size, signal;
      unsigned long int count;
      RingBuffer<ElementType> ring_buffer;
      ElementType data;

    };

}

#endif
