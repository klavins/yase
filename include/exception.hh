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