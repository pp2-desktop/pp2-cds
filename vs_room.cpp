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
    { "uid", user->get_uid_to_string()}
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
	{ "uid", opponent_user_ptr->get_uid_to_string() },
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

bool vs_room::start_round() {
  int round_cnt = vs_round_info_.current_round;

  json11::Json res = json11::Json::object {
    { "type", "start_round_res" },
    { "round_cnt", round_cnt },
  };
  master_user_ptr->send2(res);
  opponent_user_ptr->send2(res);

  return true;
}

void vs_room::find_spot(int round_cnt, int index, VS_PLAY_WINNER_TYPE winner_type) {
  std::lock_guard<std::mutex> lock(m);
  
  if(round_cnt != vs_round_info_.current_round) {
    std::cout << "[error] 유저가 보낸 라운드랑 현재 라운드랑 다름" << std::endl;
    return;
  }

  
  auto round = vs_round_info_.get_round();
  if(round) {
    bool is_find = round->is_find_spot_user(index, winner_type);
  
    // 만약 유저가 찾았다면 이번이 마지막인지 확인해야함
    if (is_find) {
      std::cout << "[debug] 유저가 찾음" << std::endl;
      // 라운드가 종료 했는지 체크
      bool is_end_round = round->check_end_round();
      if (is_end_round) {
	VS_PLAY_WINNER_TYPE winner = round->get_winner();
      } else {
	json11::Json res = json11::Json::object {
	  { "type", "find_spot_res" },
	  { "round_cnt", round_cnt },
	  { "index", index },
	  { "winner_type", winner_type },
	  { "is_end_round" , false },
	  { "is_end_vs_play" , false }
	};
	master_user_ptr->send2(res);
	opponent_user_ptr->send2(res);
      }
    } else {
      std::cout << "[debug] 유저가 못찾음" << std::endl;
      // 유저가 못찾으면 답장 안해줌?
    }
  } else {
    std::cout << "[debug] 좆됨" << std::endl;
  }
  
}

void vs_round_info::set_round_info() {

  std::unique_ptr<vs_round> vr(new vs_round);

  vr->winner = UNKNOWN;
  vr->img = "test.jpg";

  vr->vpoints.push_back(vec2(10, 11));
  vr->vpoints.push_back(vec2(10, 20));    
  vr->vpoints.push_back(vec2(20, 30));

  for(unsigned i=0; i<vr->vpoints.size(); i++) {
    vr->find_spot_users.push_back(UNKNOWN);
  }
  std::cout << "rounds size: " << rounds.size() << std::endl;

  rounds.push_back(std::move(vr));

  std::cout << "xxxxxxxxxxxxxxxxxxxxxx" << std::endl;
  //rounds.emplace_back(vr);
}

void vs_round_info::pre_loading_round_info() {
  std::lock_guard<std::mutex> lock(m);
  if (rounds.empty()) {
    std::cout << "[debug] 라운드 로딩" << std::endl;
    for(auto i=0; i<5; i++) {
      // loading
      set_round_info();
    }
  } else {
    std::cout << "[debug] 라운드가 미리 로딩되 있음" << std::endl;
  }
}

bool vs_round::is_find_spot_user(int index, VS_PLAY_WINNER_TYPE winner_type) {
  //std::lock_guard<std::mutex> lock(m);
  if(find_spot_users.size() < static_cast<unsigned>(index)) {
    std::cout << "[error] is_find_spot_user 문제 발생" << std::endl;
    return false;
  }

  if(find_spot_users[index] == UNKNOWN) {
    find_spot_users[index] = winner_type;
    return true;
  }

  return false;
}

bool vs_round::check_end_round() {
  for(unsigned i=0; i<find_spot_users.size(); i++) {
    if(find_spot_users[i] == UNKNOWN) {
      return false;
    }
  }
  return true;
}

VS_PLAY_WINNER_TYPE vs_round::get_winner() {
  auto master_cnt = 0;
  auto opponent_cnt = 0;
  for(unsigned i=0; i<find_spot_users.size(); i++) {
    if(find_spot_users[i] == MASTER) {
      master_cnt++;
    } else if(find_spot_users[i] == OPPONENT) {
      opponent_cnt++;
    } else {
      std::cout << "[error] get winner 문제 발생" << std::endl;
    }
  }

  if(opponent_cnt < master_cnt) {
    return MASTER;
  }
  return OPPONENT;
}
