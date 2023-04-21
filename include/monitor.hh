// 
// YASE Monitor Module Header
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

#ifndef YASE_MONITOR_H
#define YASE_MONITOR_H

#include "yase.hh"

namespace yase {


    //! Monitor some condition and run a function whenever it is true
    //! For example
    //! \code
    //! Monitor monitor();
    //! monitor.listen([&]() { return false; }, [&] () { std::cout << "This never prints\n"; });
    //! \endcode
    //! See the example in examples/CustomEvents for a complete example.
    class Monitor : public Module {

    public:

      Monitor();
      void init();
      void update();
      void listen(function<bool()> condition, function<void()> handler);

    private:

      function<bool()> _condition;
      function<void()> _handler;

    };

}

#endif
