#ifndef VS_ROOM_MD_HPP
#define VS_ROOM_MD_HPP

#include <map>
#include "vs_room.hpp"

typedef std::shared_ptr<vs_room> vs_room_ptr;  

class vs_room_md {

  std::map<long, vs_room_ptr> vs_rooms;
  std::mutex mu;
  size_t gen_cnt;

  boost::asio::io_service io_service_;
  std::shared_ptr<boost::asio::io_service::work> work_ptr_;
public:  

  vs_room_md();
  ~vs_room_md();

  void close();
  void run(size_t tcnt);

  static vs_room_md& get() {
    static vs_room_md obj;
    return obj;
  }
  
  //vs_room_ptr quick_join(cd_user user);
  void join_room(user_ptr user);
  void leave_room(user_ptr user);


  void join_tmp_room(user_ptr user) {
    std::lock_guard<std::mutex> lock(mu);
    users.push_back(user);
  }
  std::vector<user_ptr> users;

  boost::asio::io_service& get_io_service();
};

#endif
