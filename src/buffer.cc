// 
// YASE Buffer Module Implementation
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

#include "buffer.hh"
#include "yase.hh"

namespace yase {

  Buffer::Buffer(int size) : ring_buffer(size) {
      signal = add_input("signal");
  }

  void Buffer::init() {
      count = 0;
  }

  void Buffer::update() {
      std::get<0>(data) = count++;
      std::get<1>(data) = inputs[signal];
      ring_buffer.write(&data, 1);
  }

  int Buffer::get_buffered_output(ElementType * result, int max) {
      return ring_buffer.read(result, max);
  }

}
