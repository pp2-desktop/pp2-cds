#include "cd_user_md.hpp"
#include "vs_room_md.hpp"
#include <thread>
#include <chrono>

cd_user_md::cd_user_md(): is_on_(true) {

}

cd_user_md::~cd_user_md() {

}

void cd_user_md::check_alive() {

  std::thread t([this](std::atomic<bool>& is_on) { 

      while(is_on) {
	auto n = time(NULL);
	{
	  std::lock_guard<std::mutex> lock(m);
	  for (auto& kv : users_) {
	    //std::cout << kv.first << " has value " << kv.second << std::endl;
	    if(static_cast<time_t>(n - kv.second->get_alive_t()) > 30) {
	      std::cout << "[debug] 유저 30초 동안 아무반응 없음" << std::endl;
	      auto user = kv.second;
	      if(user->get_is_seat()) {
		vs_room_md::get().leave_room(user);
	      }
	    }
	  }
	}
	
	std::this_thread::sleep_for(std::chrono::seconds(30));
      }
    }, std::ref(this->is_on_));
  t.detach();
}

bool cd_user_md::add_user(size_t uid, user_ptr user) {
  std::lock_guard<std::mutex> lock(m);
  return users_.insert(std::make_pair(uid, user)).second;
}

bool cd_user_md::remove_user(size_t uid) {
  std::lock_guard<std::mutex> lock(m);
  return true;
}

bool cd_user_md::get_users_size() {
  return users_.size();
}
