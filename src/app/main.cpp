/**
 * @file openxum/app/main.cpp
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2011-2019 Openxum Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <restbed>

#include <nlohmann/json.hpp>

#include <openxum/core/games/kamisado/engine.hpp>
#include <openxum/core/games/kamisado/game_type.hpp>
#include <openxum/ai/specific/kamisado/mcts_player.hpp>
#include <openxum/ai/specific/kamisado/random_player.hpp>

using json = nlohmann::json;
using namespace restbed;

//std::map<int, openxum::core::common::Player *> current_players;
//int ID = 0;

//void delete_method_handler(const std::shared_ptr<Session> & /* session */)
//{
//}
//
////void get_method_handler(const std::shared_ptr<Session> &session)
////{
////  const auto request = session->get_request();
////  size_t content_length = 0;
////
////  request->get_header("Content-Length", content_length);
////  session->fetch(content_length,
////                 [request, content_length](const std::shared_ptr<Session> &session,
////                                           const Bytes &body) {
////                   std::string str = reinterpret_cast<const char *>(body.data());
////                   json data = json::parse(str.substr(0, content_length));
////                   int id = data["id"].get<int>();
////                   std::string json;
////                   openxum::core::common::Player *player = current_players[id];
////
////                   if (player != nullptr) {
////                     openxum::core::common::Move *move = player->get_move();
////
////                     if (move != nullptr) {
////                       player->move(move);
////                       json = move->to_object().dump();
////                     }
////                   }
////                   return session->close(OK, json, {{"Content-Length", std::to_string(json.size())},
////                                                    {"Content-Type", "application/json"}});
////                 });
////}
//
//void option(const std::shared_ptr<Session> & /* session */)
//{
//}
//
//void post_method_handler(const std::shared_ptr<Session> &session)
//{
//  const auto request = session->get_request();
//  size_t content_length = 0;
//
//  request->get_header("Content-Length", content_length);
//  session->fetch(content_length,
//                 [request, content_length](const std::shared_ptr<Session> &session,
//                                           const Bytes &body) {
//                   std::string str = reinterpret_cast<const char *>(body.data());
//                   json data = json::parse(str.substr(0, content_length));
//                   std::string game = data["game"].get<std::string>();
//                   int game_type = data["type"].get<int>();
//                   int color = data["color"].get<int>();
//                   int player_color = data["player_color"].get<int>();
//                   int opponent_color = data["opponent_color"].get<int>();
//                   openxum::core::common::Player *player = nullptr;
//
//                   if (game == "Kamisado") {
////                    player = new openxum::ai::specific::kamisado::RandomPlayer(player_color, opponent_color,
////                            new openxum::core::games::kamisado::Engine(game_type, color));
//                     player = new openxum::ai::specific::kamisado::MCTSPlayer(player_color,
//                                                                              opponent_color,
//                                                                              new openxum::core::games::kamisado::Engine(
//                                                                                  game_type,
//                                                                                  color));
//                   } else if (game == "Kikotsoka") {
////                    player = new openxum::ai::specific::kikotsoka::RandomPlayer(player_color, opponent_color,
////                            new openxum::core::games::kikotsoka::Engine(game_type, color));
//                     player = new openxum::ai::specific::kikotsoka::MCTSPlayer(player_color,
//                                                                               opponent_color,
//                                                                               new openxum::core::games::kikotsoka::Engine(
//                                                                                   game_type,
//                                                                                   color),
//                                                                               1000,
//                                                                               false);
//                   } else if (game == "KikotsokaPolyomino") {
////                    player = new openxum::ai::specific::kikotsoka_polyomino::RandomPlayer(player_color, opponent_color,
////                            new openxum::core::games::kikotsoka_polyomino::Engine(game_type, color));
//                     player = new openxum::ai::specific::kikotsoka_polyomino::MCTSPlayer(
//                         player_color,
//                         opponent_color,
//                         new openxum::core::games::kikotsoka_polyomino::Engine(game_type, color),
//                         1000,
//                         false);
//                   }
//
//                   if (player != nullptr) {
//                     json response;
//                     std::string json;
//
//                     ++ID;
//                     current_players[ID] = player;
//
//                     response["id"] = ID;
//                     json = response.dump();
//                     session->close(OK, json, {{"Content-Length", std::to_string(json.size())},
//                                               {"Content-Type", "application/json"}});
//                   } else {
//                     session->close(OK);
//                   }
//                 });
//}
//
//void put_method_handler(const std::shared_ptr<Session> &session)
//{
//  const auto request = session->get_request();
//  size_t content_length = 0;
//
//  request->get_header("Content-Length", content_length);
//  session->fetch(content_length,
//                 [request, content_length](const std::shared_ptr<Session> &session,
//                                           const Bytes &body) {
//                   std::string str = reinterpret_cast<const char *>(body.data());
//                   json data = json::parse(str.substr(0, content_length));
//                   int id = data["id"].get<int>();
//                   json json_move = json::parse(data["move"].get<std::string>());
//                   openxum::core::common::Player *player = current_players[id];
//
//                   if (player != nullptr) {
//                     openxum::core::common::Move *move = player->engine().build_move();
//
//                     move->from_object(json_move);
//                     player->move(move);
//                     session->close(OK);
//                   } else {
//                     session->close(OK);
//                   }
//                 });
//}
//
//void service_ready_handler(Service &)
//{
//  std::cerr << "The service is up and running." << std::endl;
//}

double findScore(openxum::ai::specific::kamisado::MCTSPlayer *player, unsigned int level) {
    player->init_search();
    for (unsigned int i = 0; i < level; ++i) {
        player->simulate_one_game_from_root();
    }
    return player->get_final_choice()->compute_score();
}

int main(int, const char **) {
//  auto GET = std::make_shared<Resource>();
//  GET->set_path("/openxum/move/get/");
//  GET->set_method_handler("GET", {{"Content-Type", "application/json"}}, get_method_handler);
//
//  auto POST = std::make_shared<Resource>();
//  POST->set_path("/openxum/game/create/");
//  POST->set_method_handler("POST", {{"Content-Type", "application/json"}}, post_method_handler);
//
//  auto PUT = std::make_shared<Resource>();
//  PUT->set_path("/openxum/move/apply/");
//  PUT->set_method_handler("PUT", {{"Content-Type", "application/json"}}, put_method_handler);
//
//  auto DELETE = std::make_shared<Resource>();
//  DELETE->set_path("/openxum/game/delete/");
//  DELETE
//      ->set_method_handler("DELETE", {{"Content-Type", "application/json"}}, delete_method_handler);
//
//  auto settings = std::make_shared<Settings>();
//  settings->set_port(1984);
//  settings->set_default_header("Connection", "close");
//
//  auto service = std::make_shared<Service>();
//  service->publish(GET);
//  service->publish(POST);
//  service->publish(PUT);
//  service->publish(DELETE);
//  service->set_ready_handler(service_ready_handler);
//  service->start(settings);

//    auto *player = new openxum::ai::specific::kamisado::MCTSPlayer(0, 1,
//                                                                   new openxum::core::games::kamisado::Engine(game_type,
//                                                                                                              color));

    std::vector<int> stats;

    const unsigned int partyNumber = 10;


    for (int i = 0; i < partyNumber; ++i) {

        auto *engine = new openxum::core::games::kamisado::Engine(
                openxum::core::games::kamisado::SIMPLE,
                openxum::core::games::kamisado::Color::BLACK);

        auto
                *player_one = new openxum::ai::specific::kamisado::MCTSPlayer(
                openxum::core::games::kamisado::Color::BLACK, openxum::core::games::kamisado::Color::WHITE,
                engine, 2);

        auto
                *player_two = new openxum::ai::specific::kamisado::MCTSPlayer(
                openxum::core::games::kamisado::Color::WHITE, openxum::core::games::kamisado::Color::BLACK,
                engine, 100);

        auto *current_player = player_one;

        while (not engine->is_finished()) {
            openxum::core::common::Move<openxum::core::games::kamisado::Decision>
                    move = current_player->get_move();

//            int color = engine->current_color();

            engine->move(move);
//        std::cout << color << " " << move.to_string() << std::endl;

            if (engine->current_color() == player_one->color()) {
                current_player = player_one;
                std::cout << " ONE "<<std::endl;
                if (!engine->is_finished())
                    std::cout << findScore(current_player, 100) << std::endl;

            } else {

                current_player = player_two;
                std::cout << " TWO "<<std::endl;
                if (!engine->is_finished())
                    std::cout << findScore(current_player, 100) << std::endl;
            }
        }

//    std::cout << engine->to_string() << std::endl;


        stats.push_back(engine->winner_is());
        std::cout << (engine->winner_is() == openxum::core::games::kamisado::Color::BLACK ? "b" : "w") << std::endl;
        delete player_one;
        delete player_two;
        delete engine;

    }

    int black = std::count_if(stats.begin(), stats.end(),
                              [](const int &i) { return i == openxum::core::games::kamisado::Color::BLACK; });
    int white = std::count_if(stats.begin(), stats.end(),
                              [](const int &i) { return i == openxum::core::games::kamisado::Color::WHITE; });

    std::cout << white << " " << black << std::endl;
    std::cout << (double) white * (double) 100 / (double) partyNumber << " "
              << (double) black * (double) 100 / (double) partyNumber << std::endl;


    return EXIT_SUCCESS;
}