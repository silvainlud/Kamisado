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

std::ofstream output_file;
std::mutex mutex;

void play(int a, int b, int c, int d, int e)
{
  openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].size = a;
  openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].piece_number = b;
  openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].shido_number = c;

  openxum::core::games::kikotsoka::Engine *engine = new openxum::core::games::kikotsoka::Engine(
      openxum::core::games::kikotsoka::SMALL,
      openxum::core::games::kikotsoka::Color::BLACK, d);
  openxum::core::common::Player<openxum::core::games::kikotsoka::Decision>
      *player_one = new openxum::ai::specific::kikotsoka::MCTSPlayer(
      openxum::core::games::kikotsoka::Color::BLACK, openxum::core::games::kikotsoka::Color::WHITE,
      engine, e, false);
//    openxum::core::common::Player* player_one = new openxum::ai::specific::kikotsoka::RandomPlayer(
//            openxum::core::games::kikotsoka::Color::BLACK, openxum::core::games::kikotsoka::Color::WHITE,
//            engine);
  openxum::core::common::Player<openxum::core::games::kikotsoka::Decision>
      *player_two = new openxum::ai::specific::kikotsoka::MCTSPlayer(
      openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
      engine, e, false);
//  openxum::core::common::Player *player_two = new openxum::ai::specific::kikotsoka::RandomPlayer(
//      openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
//      engine);
  openxum::core::common::Player<openxum::core::games::kikotsoka::Decision>
      *current_player = player_one;
  unsigned int possible_move_number = 0;
  unsigned int turn_number = 0;

  std::map<int, std::vector<int>> sizes;
  std::map<int, std::vector<double>> gains;
  std::vector<std::string> moves;
  std::map<int, std::vector<int>> levels;
  std::map<int, std::vector<std::string>> patterns;
  std::map<int, std::vector<double>> distances;
  std::map<int, std::vector<unsigned int>> distance_evaluations;

  sizes[openxum::core::games::kikotsoka::Color::BLACK] = std::vector<int>();
  sizes[openxum::core::games::kikotsoka::Color::WHITE] = std::vector<int>();
  gains[openxum::core::games::kikotsoka::Color::BLACK] = std::vector<double>();
  gains[openxum::core::games::kikotsoka::Color::WHITE] = std::vector<double>();
  patterns[openxum::core::games::kikotsoka::Color::BLACK] = std::vector<std::string>();
  patterns[openxum::core::games::kikotsoka::Color::WHITE] = std::vector<std::string>();

  while (not engine->is_finished()) {
    openxum::core::common::Move<openxum::core::games::kikotsoka::Decision>
        move = current_player->get_move();
    int color = engine->current_color();

    possible_move_number += engine->get_possible_move_list().size();
    sizes[engine->current_color()].push_back(engine->get_possible_move_list().size());
    moves.push_back(move.encode());

//    std::cout << turn_number << ": " << move.to_string() << std::endl;

    engine->move(move);

    if (engine->_pattern_origin.is_valid()) {
      patterns[color].push_back(engine->_pattern_origin.to_string());
    } else {
      patterns[color].push_back("");
    }

    if (engine->current_color() == player_one->color()) {
      current_player = player_one;
    } else {
      current_player = player_two;
    }
    if (color != engine->current_color()) {
      int opponent_color = engine->current_color() == openxum::core::games::kikotsoka::Color::BLACK
                           ? openxum::core::games::kikotsoka::Color::WHITE
                           : openxum::core::games::kikotsoka::Color::BLACK;

      gains[engine->current_color()].push_back(engine->gain(engine->current_color(), false));
      gains[opponent_color].push_back(engine->gain(opponent_color, false));
      levels[openxum::core::games::kikotsoka::Color::BLACK].push_back(engine->_black_level);
      levels[openxum::core::games::kikotsoka::Color::WHITE].push_back(engine->_white_level);
      if (color == openxum::core::games::kikotsoka::Color::BLACK) {
        distances[color].push_back(player_one->get_next_goal_distance());
        distance_evaluations[color].push_back(player_one->get_next_goal_distance_evaluation());
      } else {
        distances[color].push_back(player_two->get_next_goal_distance());
        distance_evaluations[color].push_back(player_two->get_next_goal_distance_evaluation());
      }
      ++turn_number;
    }
  }

  {
    std::unique_lock<std::mutex> lock(mutex);

    output_file << "S[BLACK];";
    for (int e: sizes[openxum::core::games::kikotsoka::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "S[WHITE];";
    for (int e: sizes[openxum::core::games::kikotsoka::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "G[BLACK];";
    for (int e: gains[openxum::core::games::kikotsoka::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "G[WHITE];";
    for (int e: gains[openxum::core::games::kikotsoka::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "D[BLACK];";
    for (int e: distances[openxum::core::games::kikotsoka::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "D[WHITE];";
    for (int e: distances[openxum::core::games::kikotsoka::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "E[BLACK];";
    for (int e: distance_evaluations[openxum::core::games::kikotsoka::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "E[WHITE];";
    for (int e: distance_evaluations[openxum::core::games::kikotsoka::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "L[BLACK];";
    for (int e: levels[openxum::core::games::kikotsoka::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "L[WHITE];";
    for (int e: levels[openxum::core::games::kikotsoka::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "P[BLACK];";
    for (const std::string &e: patterns[openxum::core::games::kikotsoka::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "P[WHITE];";
    for (const std::string &e: patterns[openxum::core::games::kikotsoka::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "X;";
    for (const std::string &m: moves) {
      output_file << m << ";";
    }
    output_file << std::endl;

    output_file << "ZB;";
    for (const int &p: engine->_black_patterns) {
      output_file << p << ";";
    }
    output_file << std::endl;
    output_file << "ZW;";
    for (const int &p: engine->_white_patterns) {
      output_file << p << ";";
    }
    output_file << std::endl;

    output_file << "T;" << a << ";" << b << ";" << c << ";"
                << engine->move_number() << ";" << possible_move_number << ";"
                << turn_number << ";"
                << engine->_black_level << ";" << engine->_white_level << ";"
                << engine->_black_captured_piece_number << ";"
                << engine->_white_captured_piece_number << ";"
                << engine->_black_captured_shido_number << ";"
                << engine->_white_captured_shido_number << ";"
                << (engine->winner_is() == openxum::core::games::kikotsoka::Color::BLACK ? "b"
                                                                                         : "w")
                << engine->_black_pattern_number << ";" << engine->_white_pattern_number << ";"
                << std::endl;
    output_file.flush();
  }

  delete player_one;
  delete player_two;
  delete engine;
}

int main(int, const char **)
{
  unsigned int max = std::thread::hardware_concurrency();
  ThreadPool pool(max);
  std::vector<std::future<void> > results;

  output_file.open("result.csv", std::ofstream::out | std::ofstream::trunc);

  for (int a = 13; a < 14; ++a) {
    for (int b = 32; b < 33; ++b) {
      for (int c = 3; c < 6; ++c) {
        for (int d = 1; d < 2; ++d) {
          for (int e = 2000; e < 2001; ++e) {
            for (int i = 0; i < 100; ++i) {
              results.emplace_back(pool.enqueue([=] { play(a, b, c, d, e); }));
            }
          }
        }
      }
    }
  }

  for (auto &&result: results) {
    result.get();
  }

  output_file.close();
  return EXIT_SUCCESS;
}