#ifndef VS_ROOM_HPP
#define VS_ROOM_HPP

#include <memory>
#include "../json11.hpp"

using namespace json11;

class cd_user;

bool sample_req(std::shared_ptr<cd_user>, Json);
//bool logout_req(std::shared_ptr<cd_user>, Json);

#endif
