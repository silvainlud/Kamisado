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



//get :
// curl -w'\n' -v -X GET 'http://localhost:1984/openxum/move'
// curl http://localhost:1984/openxum/move?game=kamisado&id=2&color=0

//post :
// curl -w'\n' -v -X POST --data 'Hello, Restbed' 'http://localhost:1984/openxum/game'

#include <iostream>
#include <restbed>

#include <nlohmann/json.hpp>

#include <openxum/core/games/kamisado/engine.hpp>
#include <openxum/core/games/kamisado/game_type.hpp>

#include <openxum/ai/specific/kamisado/random_player.hpp>

using json = nlohmann::json;
using namespace restbed;

std::map<int, openxum::core::common::Player*> current_players;
int ID = 0;

void delete_method_handler(const std::shared_ptr<Session>& /* session */)
{
}

void get_method_handler(const std::shared_ptr<Session>& session)
{
    const auto request = session->get_request();
    std::string id = request->get_query_parameter("id");
    std::string json;
    openxum::core::common::Player* player = current_players[std::stoi(id)];

    if (player != nullptr) {
        openxum::core::common::Move* move = player->get_move();

        if (move != nullptr) {
            json = move->to_object().dump();
        }
    }
    return session->close(OK, json, {{"Content-Length", std::to_string(json.size())},
                                     {"Content-Type",   "application/json"}});
}

void option(const std::shared_ptr<Session>& /* session */)
{
}

void post_method_handler(const std::shared_ptr<Session>& session)
{
    const auto request = session->get_request();
    size_t content_length = 0;

    request->get_header("Content-Length", content_length);
    session->fetch(content_length, [request](const std::shared_ptr<Session>& session, const Bytes& body) {
        json data = json::parse(body.data());
        std::string game = data["game"].get<std::string>();
        int game_type = std::stoi(data["game_type"].get<std::string>());
        int color = std::stoi(data["color"].get<std::string>());
        int opponent_color = std::stoi(data["opponent_color"].get<std::string>());
        openxum::core::common::Player* player = nullptr;

        if (game == "kamisado") {
            player = new openxum::ai::specific::kamisado::RandomPlayer(color, opponent_color,
                    new openxum::core::games::kamisado::Engine(game_type, color));
        }

        if (player != nullptr) {
            json response;
            std::string json;

            ++ID;
            current_players[ID] = player;

            response["id"] = ID;
            json = response.dump();
            session->close(OK, json, {{"Content-Length", std::to_string(json.size())},
                                      {"Content-Type",   "application/json"}});
        } else {
            session->close(OK);
        }
    });
}

void put_method_handler(const std::shared_ptr<Session>& session)
{
    const auto request = session->get_request();
    std::string id = request->get_query_parameter("id");
    openxum::core::common::Player* player = current_players[std::stoi(id)];

    if (player != nullptr) {
        size_t content_length = 0;

        request->get_header("Content-Length", content_length);
        session->fetch(content_length, [request, player](const std::shared_ptr<Session>& session, const Bytes& body) {
            json data = json::parse(body.data());
            openxum::core::common::Move* move = player->build_move();

            move->from_object(data);
            player->move(move);
            session->close(OK);
        });
    } else {
        session->close(OK);
    }
}

void service_ready_handler(Service&)
{
    fprintf(stderr, "Hey! The service is up and running.");
}

int main(int, const char**)
{
    auto GET = std::make_shared<Resource>();
    GET->set_path("/openxum/move/get/");
    GET->set_method_handler("GET", {{"Content-Type", "application/json"}}, get_method_handler);

    auto POST = std::make_shared<Resource>();
    POST->set_path("/openxum/game/create/");
    POST->set_method_handler("POST", {{"Content-Type", "application/json"}}, post_method_handler);

    auto PUT = std::make_shared<Resource>();
    PUT->set_path("/openxum/move/apply/");
    PUT->set_method_handler("PUT", {{"Content-Type", "application/json"}}, put_method_handler);

    auto DELETE = std::make_shared<Resource>();
    DELETE->set_path("/openxum/game/delete");
    DELETE->set_method_handler("DELETE", {{"Content-Type", "application/json"}}, delete_method_handler);

    auto settings = std::make_shared<Settings>();
    settings->set_port(1984);
    settings->set_default_header("Connection", "close");

    auto service = std::make_shared<Service>();
    service->publish(GET);
    service->publish(POST);
    service->publish(PUT);
    service->publish(DELETE);
    service->set_ready_handler(service_ready_handler);
    service->start(settings);

    return EXIT_SUCCESS;
}