#include <iostream>
#include <string>
#include "../cd_user.hpp"
#include "join_room.hpp"
#include <memory>
#include <thread>
#include <chrono>
#include "../vs_room_md.hpp"

static size_t cnt = 0;
bool join_room(std::shared_ptr<cd_user> user_ptr, Json payload) {
  std::cout << payload["msg"].string_value() << std::endl;

  std::cout << "받은 횟수: " << cnt++ << std::endl;


  Json my_json = Json::object {
    { "key1", "value1" },
    { "key2", false },
    { "key3", Json::array { 1, 2, 3 } },
  };

  std::string message_str = my_json.dump();


  std::cout << "사이즈: " << vs_room_md::get().users.size() << std::endl;
  for(auto a_user: vs_room_md::get().users) {

    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

    auto send_stream=std::make_shared<WsServer::SendStream>();
    *send_stream << message_str;

    a_user->server_.send(a_user->connection_, send_stream);
  }


  return true;
}
