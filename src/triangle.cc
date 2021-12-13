#include "yase.hh"

namespace yase {

  void Triangle::init() {
    update_fcn = &Triangle::ptr1;
  }

  void Triangle::update() {

  }    

  void Triangle::set_type(std::string name) {

    if ( name == "raw" )  {
      update_fcn = &Triangle::raw;
    } else if ( name == "additive" ) {
      update_fcn = &Triangle::additive;
    } else if ( name == "ptr1" ) {
      update_fcn = &Triangle::ptr1;
    } else {
      update_fcn = &Triangle::ptr1;
    }

  }    

  void Triangle::raw() {

  }    

  void Triangle::ptr1() {

  }    

  void Triangle::additive() {

  }    

}

