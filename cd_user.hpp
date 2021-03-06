#ifndef CD_USER_HPP
#define CD_USER_HPP

#include <memory>
#include "server_ws.hpp"
#include "json11.hpp"
typedef SocketServer<WS> WsServer;
/*
class room_info {
public:  
  room_info() {
    is_ready = false;
    is_room_master = false;
  }
  virtual ~room_info() {

  }

  bool is_ready;
  bool is_room_master;
  
};
*/
class vs_room;
typedef std::shared_ptr<vs_room> vs_room_ptr; 

class cd_user: public std::enable_shared_from_this<cd_user> { 

  std::atomic<bool> is_seat_;
  bool is_master_;
  vs_room_ptr vs_room_ptr_;

  //std::string uid_;
  size_t uid_;
  size_t alive_t_;
  
public:
  cd_user(WsServer& server, std::shared_ptr<WsServer::Connection> connection_ptr);
  ~cd_user();

  WsServer& server_;
  //std::weak_ptr<WsServer::Connection> connection_;
  std::shared_ptr<WsServer::Connection> connection_;

  std::string name;


  void set_is_seat(bool);
  bool get_is_seat();
 
  void set_is_master(bool);
  bool get_is_master();

  void set_vs_room(vs_room_ptr r);
  void destory();

  vs_room_ptr& get_vs_room() { return vs_room_ptr_; }
  vs_room* get_vs_room_ptr() { return vs_room_ptr_.get(); }

  void send(std::string payload);
  void send2(json11::Json payload);

  void destroy_vs_room();

  void set_uid(const size_t& uid) { uid_ = uid; }  
  size_t get_uid() { return uid_; }
  std::string get_uid_to_string() { return std::to_string(uid_); }
  time_t get_alive_t() { return alive_t_; }
  void update_alive_t() { alive_t_ = time(NULL); }
};

#endif

