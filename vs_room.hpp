#ifndef VS_ROOM_HPP
#define VS_ROOM_HPP

#include <mutex>
#include <vector>
#include <boost/asio.hpp>
#include "cd_user.hpp"
#include "json11.hpp"

using namespace json11;

typedef std::shared_ptr<cd_user> user_ptr; 

struct vec2 {
  int x;
  int y;
  vec2(int _x, int _y) {
    x = _x;
    y = _y;
  }
  Json to_json() const { return Json::array { x, y }; }
};

class round {
  std::string img;
  int diff_cnt;
  std::vector<vec2> diff_positions;
public:
  round() {
    img = "";
    diff_cnt = -1;
  }

  bool loading_round() {
    for(auto i=0; i<5; i++) {
      diff_positions.push_back(vec2(10, 10));
    }
    return true;
  }

};

class vs_room: public std::enable_shared_from_this<vs_room> {

  enum ROOM_STATUS { LOBBY, PLAYING, END };

  std::mutex m;
  ROOM_STATUS room_status;
  int rid_;

  bool is_opponent_ready_;
  boost::asio::io_service& io_service_;
  
public:
  user_ptr master_user_ptr;
  user_ptr opponent_user_ptr;

  vs_room(boost::asio::io_service& io_service, int rid);
  ~vs_room();


  bool is_full();
  int get_id();

  void join_as_master(user_ptr user);
  void join_as_opponent(user_ptr user);

  void leave_user(user_ptr user);

  bool start();
  void reset();

  bool set_is_opponent_ready();
  
};

#endif
