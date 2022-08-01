// 
// YASE Exception Header
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

#ifndef _YASE_EXCEPTIONS_H
#define _YASE_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace yase {

    using std::string;

    //! An exception class for Yase.

    //! Exception messages will be prefixed with "Yase error: "
    //! but otherwise look like run time errors.
    class Exception : public std::runtime_error {
       public:
       Exception(const string& what = "") : std::runtime_error("Yase Error: " + what) {}
    };

}

#endif