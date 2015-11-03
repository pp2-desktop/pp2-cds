#include "cd_user.hpp"
#include "vs_room.hpp"
#include <iostream>
#include "vs_room_md.hpp"


typedef std::shared_ptr<cd_user> user_ptr;  

cd_user::cd_user(WsServer& server, std::shared_ptr<WsServer::Connection> connection_ptr) : server_(server), connection_(connection_ptr) {
  std::cout << "유저 생성자 called" << std::endl;
  name = "helloworld";
  is_seat_ = false;
  is_master_ = false;
  vs_room_ptr_ = nullptr;
}

cd_user::~cd_user() {
  std::cout << "유저 소멸자 called" << std::endl;
}

void cd_user::set_is_seat(bool is_seat) {
  is_seat_ = is_seat;
}

bool cd_user::get_is_seat() {
  return is_seat_;
}

void cd_user::set_is_master(bool is_master) {
  is_master_ = is_master;
}

bool cd_user::get_is_master() {
  return is_master_;
}

void cd_user::set_vs_room(vs_room_ptr r) {
  vs_room_ptr_ = r;
}

void cd_user::destory() {
  std::cout << "[debug] user destory called" << std::endl;
  if(is_seat_) {
    vs_room_md::get().leave_room(shared_from_this());
  }
}

void cd_user::send(std::string payload) {
  auto send_stream=std::make_shared<WsServer::SendStream>();
  *send_stream << payload;
  server_.send(connection_, send_stream);
}

void cd_user::destroy_vs_room() {
  is_seat_ = false;
  is_master_ = false;
  vs_room_ptr_ = nullptr;
}
