#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <vector>
#include "vs_play_req.hpp"
#include "../cd_user.hpp"
#include "../vs_room.hpp"

using namespace json11;

bool round_info_req(std::shared_ptr<cd_user> user_ptr, Json) {

  std::vector<Json> round_infos;

  for(auto i=0; i<5; i++) {
    
    std::string img0 = "test.png";
    std::string img1 = "test2.png";

    std::vector<int> points;
    points.push_back(10);
    points.push_back(11);
    points.push_back(10);
    points.push_back(12);

    Json round = Json::object({
	{ "img0", img0 },
	{ "img1", img1 },
	{ "point_infos", points } 
      });

    round_infos.push_back(round);
  }

  Json tmp = round_infos;
 
   
  Json res = Json::object({
    { "type", "round_info_res" },
    { "round_infos", tmp }
    });
  
  user_ptr->send2(res);

  return true;
}

/*
bool opponent_ready_notify(std::shared_ptr<cd_user> user_ptr, Json) {

  auto room_ptr = user_ptr->get_vs_room_ptr();
  
  if(room_ptr) {
    auto master_user_ptr = room_ptr->master_user_ptr;
    if(master_user_ptr) {
      master_user_ptr->send2(json11::Json::object {
	  { "type", "opponent_ready_notify" }
	});
    } 
  }
  return true;
}

bool start_vs_game_req(std::shared_ptr<cd_user> user_ptr, Json) {

  std::cout << "게임 시작 메시지 받음" << std::endl;

  auto room_ptr = user_ptr->get_vs_room_ptr();

  auto res = json11::Json::object{ 
    { "type", "start_vs_game_res" }
  };
  
  if(room_ptr) {
    auto master_user_ptr = room_ptr->master_user_ptr;
    if(master_user_ptr) {
      master_user_ptr->send2(res);
    }
    auto opponent_user_ptr = room_ptr->master_user_ptr;
    if(opponent_user_ptr) {
      opponent_user_ptr->send2(res);
    }
  }
  
  return true;
}
*/
