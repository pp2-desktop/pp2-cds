#include "cd_user_md.hpp"
#include "vs_room_md.hpp"
#include <thread>
#include <chrono>

cd_user_md::cd_user_md(): is_on_(true) {

}

cd_user_md::~cd_user_md() {

}

void cd_user_md::start_check_alive() {
  while(is_on_) {
    auto n = time(NULL); 
    {
      std::lock_guard<std::mutex> lock(m);
      for (auto& kv : users_) {
	auto user = kv.second;
	//std::cout << kv.first << " has value " << kv.second << std::endl;
	if(static_cast<time_t>(n - kv.second->get_alive_t()) > 30) {
	  if(user->get_is_seat()) {
	    json11::Json res = json11::Json::object {
	      { "type", "kick_user_notify" },
	      { "reason", "alive_t_expired" }
	    };
	    user->send2(res);
	    vs_room_md::get().leave_room(user);
	  }
	} else {
	  if(user->get_is_seat()) {
	    json11::Json res = json11::Json::object {
	      { "type", "update_alive_res" }
	    };
	    user->send2(res);
	    std::cout << "[debug] 유저에게 update_alive_res 보냄" << std::endl;
	  }
	}
      }
    }
    std::cout << "[debug] wait for 30 sec" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(30));
  }
	 
}

void cd_user_md::stop_check_alive() {
  is_on_ = false;
}

bool cd_user_md::add_user(size_t uid, user_ptr user) {
  std::lock_guard<std::mutex> lock(m);
  return users_.insert(std::make_pair(uid, user)).second;
}

bool cd_user_md::remove_user(size_t uid) {
  std::lock_guard<std::mutex> lock(m);

  auto it = users_.find(uid);
  if(it == users_.end()) {
    return false;
  }
  users_.erase (it);   
  
  return true;
}

bool cd_user_md::get_users_size() {
  return users_.size();
}
