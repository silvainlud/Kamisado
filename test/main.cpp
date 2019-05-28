/**
 * @file test/main.cpp
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
#include <fstream>
#include "thread_pool.hpp"

#include <openxum/core/games/kikotsoka/engine.hpp>
#include <openxum/core/games/kikotsoka/game_type.hpp>
#include <openxum/ai/specific/kikotsoka/mcts_player.hpp>
#include <openxum/ai/specific/kikotsoka/random_player.hpp>
//#include <chrono>

std::ofstream output_file("result");

void test_mcts()
{
    openxum::core::games::kikotsoka::Engine* engine = new openxum::core::games::kikotsoka::Engine(
            openxum::core::games::kikotsoka::SMALL,
            openxum::core::games::kikotsoka::Color::BLACK);
    openxum::core::common::Player* player_one = new openxum::ai::specific::kikotsoka::MCTSPlayer(
            openxum::core::games::kikotsoka::Color::BLACK, openxum::core::games::kikotsoka::Color::WHITE,
            engine, 10, false);
//    openxum::core::common::Player* player_two = new openxum::ai::specific::kikotsoka::MCTSPlayer(
//            openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
//            engine, 2000, true);
    openxum::core::common::Player* player_two = new openxum::ai::specific::kikotsoka::RandomPlayer(
            openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
            engine);
    openxum::core::common::Player* current_player = player_one;

    while (not engine->is_finished()) {

//        auto t0 = std::chrono::high_resolution_clock::now();

        openxum::core::common::Move* move = current_player->get_move();

//        auto t1 = std::chrono::high_resolution_clock::now();

//        std::cout << "duration = " << double((t1 - t0).count()) / 1000000000. << std::endl;

        engine->move(move);
        if (engine->current_color() == player_one->color()) {
            current_player = player_one;
        } else {
            current_player = player_two;
        }
    }

    if (engine->winner_is() == openxum::core::games::kikotsoka::Color::BLACK) {
        std::cout << "Winner is BLACK" << std::endl;
    } else {
        std::cout << "Winner is WHITE" << std::endl;
    }

    delete player_one;
    delete player_two;
    delete engine;
}

void play(unsigned int a, unsigned int b)
{
    openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].size = a;
    openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].piece_number = b;

    openxum::core::games::kikotsoka::Engine* engine = new openxum::core::games::kikotsoka::Engine(
            openxum::core::games::kikotsoka::SMALL,
            openxum::core::games::kikotsoka::Color::BLACK);
    openxum::core::common::Player* player_one = new openxum::ai::specific::kikotsoka::RandomPlayer(
            openxum::core::games::kikotsoka::Color::BLACK, openxum::core::games::kikotsoka::Color::WHITE,
            engine);
    openxum::core::common::Player* player_two = new openxum::ai::specific::kikotsoka::MCTSPlayer(
            openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
            engine, 1000, false);
//    openxum::core::common::Player* player_two = new openxum::ai::specific::kikotsoka::RandomPlayer(
//            openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
//            engine);
    openxum::core::common::Player* current_player = player_one;
    unsigned int possible_move_number = 0;

    while (not engine->is_finished()) {
        openxum::core::common::Move* move = current_player->get_move();

        possible_move_number += engine->get_possible_move_list().size();
        engine->move(move);
        if (engine->current_color() == player_one->color()) {
            current_player = player_one;
        } else {
            current_player = player_two;
        }
        delete move;
    }

    output_file << a << ";" << b << ";" << engine->move_number() << ";"
              << double(possible_move_number) / engine->move_number() << ";"
              << engine->_white_level << ";" << engine->_black_level << ";"
              << std::endl;

    delete player_one;
    delete player_two;
    delete engine;
}

void test_random()
{
    unsigned int max = std::thread::hardware_concurrency();
    ThreadPool pool(max);
    std::vector<std::future<void> > results;

    for (unsigned int a = 15; a < 16; ++a) {
        for (unsigned int b = 50; b < 51; ++b) {
            for (unsigned int i = 0; i < 20; ++i) {
                results.emplace_back(pool.enqueue([=] { play(a, b); }));
            }
        }
    }
    for (auto&& result: results) {
        result.get();
    }
}

int main(int, const char**)
{
//    test_mcts();
    test_random();
    output_file.close();
    return EXIT_SUCCESS;
}