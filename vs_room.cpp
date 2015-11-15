#include "vs_room.hpp"
#include "json11.hpp"

vs_room::vs_room(boost::asio::io_service& io_service, int rid) : io_service_(io_service) {
  master_user_ptr = nullptr;
  opponent_user_ptr = nullptr;
  rid_ = rid;
  reset();
}

vs_room::~vs_room() {

}


bool vs_room::start() {


  return true;
}


bool vs_room::is_full() {
  if(master_user_ptr == nullptr || opponent_user_ptr == nullptr) {
    return false;
  }
  return true ;
}

int vs_room::get_id() {
  return rid_;
}

void vs_room::join_as_master(user_ptr user) {
  master_user_ptr = user;

  json11::Json res = json11::Json::object {
   { "type", "join_room_res" },
   { "result", true },
   { "is_master", true },
   { "rid", rid_}
  };

  user->set_is_seat(true);
  user->set_is_master(true);
  user->set_vs_room(shared_from_this());
  
  user->send(res.dump());
}

void vs_room::join_as_opponent(user_ptr user) {
  opponent_user_ptr = user;

  json11::Json res = json11::Json::object {
   { "type", "join_room_res" },
   { "result", true },
   { "is_master", false },
   { "rid", rid_}
  };

  user->set_is_seat(true);
  user->set_is_master(false);
  user->set_vs_room(shared_from_this());
  user->send(res.dump());


  json11::Json n = json11::Json::object {
    { "type", "join_opponent_notify" },
    { "uid", user->get_uid()}
  };
  master_user_ptr->send2(n);
}

void vs_room::leave_user(user_ptr user) {

  if(room_status == LOBBY) {
    if(master_user_ptr.get() == user.get()) {
      if(opponent_user_ptr != nullptr) { 
	master_user_ptr = opponent_user_ptr;
	opponent_user_ptr.reset();
	opponent_user_ptr = nullptr;
	
	master_user_ptr->set_is_master(true);
	//master_user_ptr 한테 너 방장됬다고 메세지 보내줌

	json11::Json n = json11::Json::object {
	  { "type", "master_leave_notify" },
	  { "is_master", true },
	};
	master_user_ptr->send(n.dump());

	std::cout << "[debug] 방장이 나가서 상대가 방장이 됨" << std::endl;
      } else {

	std::cout << "[debug] 방장이 나가서 방 삭제" << std::endl;
	master_user_ptr.reset();
	master_user_ptr = nullptr;
	opponent_user_ptr.reset();
	opponent_user_ptr = nullptr;
      }

    } else {
      std::cout << "[debug] 상대가 나가고 방장만 혼자 있는 상태" << std::endl;

      json11::Json n = json11::Json::object {
	{ "type", "opponent_leave_notify" },
	{ "uid", opponent_user_ptr->get_uid() },
      };
      master_user_ptr->send2(n);

      opponent_user_ptr.reset();
      opponent_user_ptr = nullptr;
    }

    user->destroy_vs_room();
  } else {
    
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


void vs_room::change_status(ROOM_STATUS rs) {
  std::lock_guard<std::mutex> lock(m);
  room_status = rs;
}
