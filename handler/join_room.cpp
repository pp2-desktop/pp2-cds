#include <iostream>
#include <string>
#include "../cd_user.hpp"
#include "join_room.hpp"
#include <memory>
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
  auto send_stream=std::make_shared<WsServer::SendStream>();
  *send_stream << message_str;

  user_ptr->server_.send(user_ptr->connection_, send_stream, [](const boost::system::error_code& ec) {
            if(ec) {
	      std::cout << "Server: Error sending message. " <<
                //See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
		"Error: " << ec << ", error message: " << ec.message() << std::endl;
            }
        });

  return true;
}


