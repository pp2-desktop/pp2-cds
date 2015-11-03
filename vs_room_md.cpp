#include "vs_room_md.hpp"
#include <utility>    

vs_room_md::vs_room_md() {
  std::cout << "vs_room_md 생성자 called" << std::endl;
  gen_cnt = 100;
}


vs_room_md::~vs_room_md() {
  std::cout << "vs_room_md 소멸자 called" << std::endl;

}

void vs_room_md::close() {
  work_ptr_.reset();
}

void vs_room_md::run(size_t tcnt) {

  work_ptr_.reset( new boost::asio::io_service::work(io_service_) );

  std::vector<std::shared_ptr<std::thread>> threads;
  for (std::size_t i = 0; i < tcnt; ++i) {
    std::shared_ptr<std::thread> t( new std::thread(std::bind(  static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &io_service_)));
    threads.push_back(t);
  }

  for (std::size_t i = 0; i < threads.size(); ++i) {
    threads[i]->join();
  }

}

void vs_room_md::join_room(user_ptr user) {
  std::lock_guard<std::mutex> lock(mu);
  
  for(auto& p : vs_rooms) {
    if(!p.second->is_full()) {
      // 방에 자리가 있다면
      std::cout << "[debug] 유저가 있어서 상대로 들어감" << std::endl;
      p.second->join_as_opponent(user);
      return;
    }
  }

  auto r = std::make_shared<vs_room>(io_service_, gen_cnt);
  vs_rooms.insert(std::make_pair(gen_cnt, r));
  r->join_as_master(user);
  gen_cnt++;
}

void vs_room_md::leave_room(user_ptr user) {
  std::lock_guard<std::mutex> lock(mu);

  auto rid = user->get_vs_room()->get_id();

  auto it = vs_rooms.find(rid);
  if (it != vs_rooms.end()) {
    it->second->leave_user(user); 

  } else {
    std::cout << "[error] 유저가 방을 나갈려 했지만 방이 존재하지 않음" << std::endl; 
  }
  
  if(it != vs_rooms.end()) {
    std::cout << "[debug] 방 삭제전 사이즈: " << vs_rooms.size() << std::endl; 
    if(it->second->master_user_ptr == nullptr && it->second->opponent_user_ptr == nullptr) {
      vs_rooms.erase(it);
    } else {
      std::cout << "[debug] 방에 유저가 있어서 삭제 안함" << std::endl;       
    }
    std::cout << "[debug] 방 삭제후 사이즈: " << vs_rooms.size() << std::endl; 
  }
  
}

boost::asio::io_service& vs_room_md::get_io_service() {
  return io_service_;
}

/*
pvp_room_ptr pvp_room_md::create() {
  pvp_room_ptr p = std::make_shared<pvp_room>();
  return p;
}
*/

/*
vs_room_ptr vs_room_md::quick_join(cd_user user) {
  std::lock_guard<std::mutex> lock(m);

  for(auto& p : vs_rooms) {

    if(!p.second->is_full()) {     
      return p.second;
    }

  }
  
  gen_cnt++;
  vs_room_ptr r = std::make_shared<vs_room>(gen_cnt);
  vs_rooms.insert(std::make_pair(gen_cnt, r));

  return r;
}
*/

