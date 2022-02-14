#include <fstream>
#include "yase.hh"

namespace yase {

  json get_config ( string file_name ) {

    json config;
    std::ifstream config_stream(file_name);
    config_stream >> config;     
    return config;

  }


}

