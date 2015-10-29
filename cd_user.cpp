#include "cd_user.hpp"
#include <iostream>

typedef std::shared_ptr<cd_user> user_ptr;  

cd_user::cd_user(WsServer& server, std::shared_ptr<WsServer::Connection> connection_ptr) : server_(server), connection_(connection_ptr) {
  std::cout << "유저 생성자 called" << std::endl;
  name = "helloworld";
}

cd_user::~cd_user() {
  std::cout << "유저 소멸자 called" << std::endl;
}


