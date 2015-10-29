#ifndef CD_USER_HPP
#define CD_USER_HPP

#include <memory>
#include "server_ws.hpp"

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

class cd_user {
public:
  cd_user(WsServer& server, std::shared_ptr<WsServer::Connection> connection_ptr);
  ~cd_user();

  WsServer& server_;
  //std::weak_ptr<WsServer::Connection> connection_;
  std::shared_ptr<WsServer::Connection> connection_;

  std::string name;

};

#endif
