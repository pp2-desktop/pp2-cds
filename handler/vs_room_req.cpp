#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include "user_info_req.hpp"
#include "../cd_user.hpp"

bool sample_req(std::shared_ptr<cd_user> user_ptr, Json) {

  json11::Json res = json11::Json::object {
    { "type", "login_res" },
    { "uid", user_ptr->get_uid() }
  };

  user_ptr->send2(res);

  return true;
}

