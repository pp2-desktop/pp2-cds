#include <utility>
#include "cd_handler_md.hpp"
#include "handler/join_room.hpp"
#include <iostream>

cd_handler_md::cd_handler_md() {

}

cd_handler_md::~cd_handler_md() {

}

bool cd_handler_md::add_payload(std::string type, std::function<bool(user_ptr, Json)> handle_fn_ptr) {
  return m.insert(std::make_pair(type, handle_fn_ptr)).second;
}

bool cd_handler_md::init() {
  std::cout << "cd_handler_md init called" << std::endl;
  bool r = true;
  r &= add_payload("join_room", join_room);

  std::cout << r << std::endl;
  return r;
}
