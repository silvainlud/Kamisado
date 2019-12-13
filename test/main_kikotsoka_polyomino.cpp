/**
 * @file test/main_kikotsoka_polyomino.cpp
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

#include <openxum/core/games/kikotsoka-polyomino/engine.hpp>
#include <openxum/core/games/kikotsoka-polyomino/game_type.hpp>
#include <openxum/ai/specific/kikotsoka-polyomino/mcts_player.hpp>
#include <openxum/ai/specific/kikotsoka-polyomino/random_player.hpp>

//std::ofstream output_file("result");

void test_random()
{
    auto engine = new openxum::core::games::kikotsoka_polyomino::Engine(
            openxum::core::games::kikotsoka_polyomino::SMALL,
            openxum::core::games::kikotsoka_polyomino::Color::BLACK);

    openxum::core::common::Player<openxum::core::games::kikotsoka_polyomino::Decision>* player_one = new openxum::ai::specific::kikotsoka_polyomino::MCTSPlayer(
            openxum::core::games::kikotsoka_polyomino::Color::BLACK,
            openxum::core::games::kikotsoka_polyomino::Color::WHITE,
            engine, 100, false);

//    openxum::core::common::Player* player_one = new openxum::ai::specific::kikotsoka_polyomino::RandomPlayer(
//            openxum::core::games::kikotsoka_polyomino::Color::BLACK,
//            openxum::core::games::kikotsoka_polyomino::Color::WHITE,
//            engine);

    openxum::core::common::Player<openxum::core::games::kikotsoka_polyomino::Decision>* player_two = new openxum::ai::specific::kikotsoka_polyomino::RandomPlayer(
            openxum::core::games::kikotsoka_polyomino::Color::WHITE,
            openxum::core::games::kikotsoka_polyomino::Color::BLACK,
            engine);
    openxum::core::common::Player<openxum::core::games::kikotsoka_polyomino::Decision>* current_player = player_one;

    while (not engine->is_finished()) {
        openxum::core::common::Move<openxum::core::games::kikotsoka_polyomino::Decision> move = current_player->get_move();

        engine->move(move);

        std::cout << move.to_string() << std::endl;

        if (engine->current_color() == player_one->color()) {
            current_player = player_one;
        } else {
            current_player = player_two;
        }
    }

    if (engine->winner_is() == openxum::core::games::kikotsoka_polyomino::Color::BLACK) {
        std::cout << "Winner is BLACK" << std::endl;
    } else {
        std::cout << "Winner is WHITE" << std::endl;
    }

    delete player_one;
    delete player_two;
    delete engine;
}

int main(int, const char**)
{
    test_random();

//    output_file.close();
    return EXIT_SUCCESS;
}