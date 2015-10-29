#include "vs_room.hpp"

vs_room::vs_room(boost::asio::io_service& io_service, size_t rid) : io_service_(io_service) {
  master_user_ptr = nullptr;
  opponent_user_ptr = nullptr;
  reset();
}

vs_room::~vs_room() {

}


bool vs_room::start() {


  return true;
}


bool vs_room::is_full() {


  return true ;
}

size_t vs_room::get_id() {
  return rid_;
}

void vs_room::join_as_master(user_ptr user) {
  master_user_ptr = user;
}

void vs_room::join_as_opponent(user_ptr user) {
  opponent_user_ptr = user;
}

void vs_room::leave_user(user_ptr user) {

  if(room_status == LOBBY) {

    if(master_user_ptr.get() == user.get()) {
      master_user_ptr = opponent_user_ptr;
      
      //master_user_ptr 한테 너 방장됬다고 메세지 보내줌

      opponent_user_ptr = nullptr;
    } else {
      opponent_user_ptr = nullptr;
    }

    reset();
  }

  
}

bool vs_room::set_is_opponent_ready() {
  if(is_opponent_ready_) return false;
  is_opponent_ready_ = true;
  return is_opponent_ready_;
}


void vs_room::reset() {
  room_status = LOBBY;
  is_opponent_ready_ = false;
}
