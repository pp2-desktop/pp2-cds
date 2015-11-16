#ifndef VS_ROOM_HPP
#define VS_ROOM_HPP

#include <mutex>
#include <vector>
#include <boost/asio.hpp>
#include "cd_user.hpp"
#include "json11.hpp"
#include <atomic>

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

enum VS_PLAY_WINNER_TYPE { MASTER, OPPONENT, UNKNOWN };

class vs_round {
public:
  std::vector<VS_PLAY_WINNER_TYPE> find_spot_user;
  VS_PLAY_WINNER_TYPE winner;
  std::string img;
  std::vector<vec2> vpoints;
  std::atomic<int> ready_cnt;

  vs_round() : ready_cnt(0) {}
  ~vs_round() {}

  
  vs_round(const vs_round& vr) {
    std::cout << "vs_round 복사 생성자 called" << std::endl;
    this->ready_cnt.store(vr.ready_cnt.load());
  }

};

class vs_round_info {
public:
  std::mutex m;

  std::vector<vs_round> rounds;
  std::atomic<int> current_round;

  vs_round_info() {
    reset();
  }

  int get_vs_current_round() { return current_round+1; }
  
  void set_round_info();
  void pre_loading_round_info();

  void reset() {
    current_round = 0;
    rounds.clear();
  }

  int get_round_ready_cnt() {
    return rounds[current_round].ready_cnt;
  }
  void inc_round_ready_cnt() {
    ++rounds[current_round].ready_cnt;
  }
};

class vs_room: public std::enable_shared_from_this<vs_room> {

private:
  std::mutex m;
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

  bool start_round();

  enum ROOM_STATUS { LOBBY, PLAYING, END };
  void change_status(ROOM_STATUS rs);  

  ROOM_STATUS room_status;
  
  vs_round_info vs_round_info_;
};

#endif
