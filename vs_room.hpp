#ifndef VS_ROOM_HPP
#define VS_ROOM_HPP

#include <mutex>
#include <vector>
#include <boost/asio.hpp>
#include "cd_user.hpp"
#include "json11.hpp"
#include <atomic>
#include <memory>

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
  std::vector<VS_PLAY_WINNER_TYPE> find_spot_users;
  VS_PLAY_WINNER_TYPE winner;
  std::string img;
  std::vector<vec2> vpoints;
  std::atomic<int> ready_cnt;
  //std::vector<bool, VS_PLAY_WINNER_TYPE> find_spots;

  vs_round() : ready_cnt(0) {}
  ~vs_round() {
    std::cout << "vs_round 소멸자 called" << std::endl;
  }

  
  vs_round(const vs_round& vr) {
    std::cout << "vs_round 복사 생성자 called" << std::endl;
    this->ready_cnt.store(vr.ready_cnt.load());
  }

  vs_round(const vs_round&& vr) {
    std::cout << "vs_round move 생성자 called" << std::endl;
    this->ready_cnt.store(vr.ready_cnt.load());
  }

  bool is_find_spot_user(int index, VS_PLAY_WINNER_TYPE winner_type);
  bool check_end_round();
  VS_PLAY_WINNER_TYPE get_winner();
  void set_winner(VS_PLAY_WINNER_TYPE round_winner) {
    winner = round_winner;
  }
 
  //VS_PLAY_WINNER_TYPE get_winner_type() { return winner_; }
};

class vs_round_info {
public:
  std::mutex m;

  std::vector<std::unique_ptr<vs_round>> rounds;
  std::atomic<int> current_round;

  vs_round_info() {
    reset();
  }

  //int get_vs_current_round() { return current_round+1; }
  
  void set_round_info();
  void pre_loading_round_info();

  void reset() {
    current_round = 0;
    rounds.clear();
  }

  int get_round_ready_cnt() {
    return rounds[current_round]->ready_cnt;
  }
  void inc_round_ready_cnt() {
    ++rounds[current_round]->ready_cnt;
  }

  vs_round* get_round() { 
    std::lock_guard<std::mutex> lock(m);
    return rounds[current_round].get();
  }

  VS_PLAY_WINNER_TYPE get_winner();

  bool check_end_vs_play() {
    if(current_round >= static_cast<int>(rounds.size())) {
      return true;
    }
    return false;
  }
  
  //void find_spot(int stage, int index, VS_PLAY_WINNER_TYPE winner_type);
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
  bool is_playing();
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

  void find_spot(int round_cnt, int index, VS_PLAY_WINNER_TYPE winner_type);
  void play_to_lobby();
  
  vs_round_info vs_round_info_;
};

#endif
