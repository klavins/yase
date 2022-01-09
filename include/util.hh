#ifndef YASE_UTIL_H
#define YASE_UTIL_H

#include "json.hpp"
#include <string>

using namespace std;
using namespace nlohmann;

namespace yase {

  json get_config ( string file_name );

}

#endif
