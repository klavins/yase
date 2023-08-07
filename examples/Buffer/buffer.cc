// 
// YASE Example
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

#include <vector>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "ring_buffer.hh"

yase::RingBuffer<double> rb(100);

void read_stuff() {
    usleep(1);
    double y[10];
    int n, m=0;
    for ( int i=0; i<300; i++ ) { 
      n = rb.read(y,20); 
      m += n;
      for ( int j=0; j<n-1; j++ ) { 
        std::cout << y[j] << std::endl;
        if(y[j] != y[j+1]-1 ) {
           std::cout << "error: " << y[j] << ", " << y[j+1] << std::endl;
        }
      }
    }
    std::cout << "read " << m << " elements" << std::endl; 
}

void write_stuff() {
    double u[10];
    int n = 0;
    for(int i=0; i<300; i++) {
        u[0] = i;
        n += rb.write(u,1);
    }   
    std::cout << "wrote " << n << " elements" << std::endl;    
}

int main() {

    std::thread reader(read_stuff),
                writer(write_stuff);

    usleep(100000);                

    reader.join();
    writer.join();

    usleep(100000);     

    return 0;
 
}