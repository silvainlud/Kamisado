//get :
// curl -w'\n' -v -X GET 'http://localhost:1984/openxum/move'
// curl http://localhost:1984/openxum/move?game=kamisado&id=2&color=0

//post :
// curl -w'\n' -v -X POST --data 'Hello, Restbed' 'http://localhost:1984/openxum/create'
 
#include <thread>
#include <memory>
#include <chrono>
#include <cstdlib>
#include <restbed>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include "nlohmann/json.hpp"
#include "game/Kamisado/Engine.h"

using json = nlohmann::json;
using namespace std;
using namespace restbed;

void option( const shared_ptr< Session > session ){
  
}

void get_method_handler( const shared_ptr< Session > session )
{
  const auto request = session->get_request( );
  string game = request->get_query_parameter("game");
  string id = request->get_query_parameter("id");
  string color = request->get_query_parameter("color");
  json j;
  string json = "";
  Engine e(0,0);
  if(game == "kamisado"){
    Coordinates playable_tower = e.find_playable_tower(atoi(color.c_str()));
    std::mt19937 rng;
    rng.seed(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,7);
    playable_tower._x = distribution(rng);
    if(atoi(color.c_str()) == 0){
      playable_tower._y = 0;
    } else {
      playable_tower._y = 7;
    }
    State s(playable_tower._x,playable_tower._y,atoi(color.c_str()));
    vector<Coordinates> list = e.get_possible_moving_list(s);
    if(list.size() > 0){
      j["from"] = to_string(playable_tower._x) + " " + to_string(playable_tower._y);
      std::uniform_int_distribution<std::mt19937::result_type> to_list(0,list.size());
      j["to"] = to_string(list[to_list(rng)]._x) + " " + to_string(list[to_list(rng)]._y);
      json = j.dump();
    }
  }
  return session->close( OK, json, { { "Content-Length", to_string(json.size()) }, { "Content-Type", "application/json" } } );
}
 
void post_method_handler( const shared_ptr< Session > session )
{
  const auto request = session->get_request( );
  size_t content_length = 0;
  request->get_header( "Content-Length", content_length );
    
  session->fetch( content_length, [request]( const shared_ptr< Session > session, const Bytes & body ){
      fprintf( stdout, "%.*s\n", (int) body.size( ), body.data( ) );
      vector<string> param;
      string tmp = "";
      for(int i = 0; i<body.size(); i++){
	if(body[i] != '"'){
	  if(int(body[i] > 96)){
	    if(int(body[i] < 123)){
	      tmp += body[i];
	    }
	  }
	  if(int(body[i] > 47)){
	    if(int(body[i] < 58)){
	      tmp += body[i];
	    }
	  }
	}else{
	  if(tmp.size() != 0){
	    param.push_back(tmp);
	    tmp = "";
	  }
	}
      }
      string id = param[5];
      json j;
      j["id"] = id;
      Engine e(0,atoi(param[3].c_str()));
      j["Engine _type"] = to_string(e._type);
      j["Engine _color"] = to_string(e._color);
      string json = j.dump();
      session->close( OK, json, { { "Content-Length", to_string(json.size()) }, { "Content-Type", "application/json" } } );
    });
}
 
void put_method_handler( const shared_ptr< Session > session )
{
  const auto request = session->get_request( );
  string game = request->get_query_parameter("game");
  string id = request->get_query_parameter("id");
}
 
void service_ready_handler( Service& )
{
    fprintf( stderr, "Hey! The service is up and running." );
}
 
int main( const int, const char** )
{
    auto GET = make_shared< Resource >( );
    GET->set_path( "/openxum/move/" );
    GET->set_method_handler( "GET", {{"Content-Type", "application/json"}}, get_method_handler );
 
    auto POST = make_shared< Resource >( );
    POST->set_path( "/openxum/create/" );
    POST->set_method_handler( "POST", {{"Content-Type", "application/json"}},post_method_handler );
 
    auto PUT = make_shared< Resource >( );
    PUT->set_path( "/openxum/move" );
    PUT->set_method_handler( "PUT", put_method_handler );
 
    auto settings = make_shared< Settings >( );
    settings->set_port( 1984 );
    settings->set_default_header( "Connection", "close" );
 
    auto service = make_shared< Service >( );
    service->publish( GET );
    service->publish( POST );
    //service->publish( PUT );
    service->set_ready_handler( service_ready_handler );
    service->start( settings );
 
    return EXIT_SUCCESS;
}
