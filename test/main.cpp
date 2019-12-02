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

std::ofstream output_file("result.csv");
std::mutex mutex;

void test_mcts()
{
  auto engine = new openxum::core::games::kikotsoka::Engine(
      openxum::core::games::kikotsoka::SMALL,
      openxum::core::games::kikotsoka::Color::BLACK);
  openxum::core::common::Player *player_one = new openxum::ai::specific::kikotsoka::MCTSPlayer(
      openxum::core::games::kikotsoka::Color::BLACK, openxum::core::games::kikotsoka::Color::WHITE,
      engine, 100, false);
//    openxum::core::common::Player* player_two = new openxum::ai::specific::kikotsoka::MCTSPlayer(
//            openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
//            engine, 2000, true);
  openxum::core::common::Player *player_two = new openxum::ai::specific::kikotsoka::RandomPlayer(
      openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
      engine);
  openxum::core::common::Player *current_player = player_one;

  while (not engine->is_finished()) {

//        auto t0 = std::chrono::high_resolution_clock::now();

    openxum::core::common::Move *move = current_player->get_move();

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

void play(int a, int b, int c)
{
  openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].size = a;
  openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].piece_number = b;
  openxum::core::games::kikotsoka::Engine::CONFIGURATIONS[0].shido_number = c;

  openxum::core::games::kikotsoka::Engine *engine = new openxum::core::games::kikotsoka::Engine(
      openxum::core::games::kikotsoka::SMALL,
      openxum::core::games::kikotsoka::Color::BLACK);
  openxum::core::common::Player *player_one = new openxum::ai::specific::kikotsoka::MCTSPlayer(
      openxum::core::games::kikotsoka::Color::BLACK, openxum::core::games::kikotsoka::Color::WHITE,
      engine, 1000, false);
//    openxum::core::common::Player* player_one = new openxum::ai::specific::kikotsoka::RandomPlayer(
//            openxum::core::games::kikotsoka::Color::BLACK, openxum::core::games::kikotsoka::Color::WHITE,
//            engine);
  openxum::core::common::Player *player_two = new openxum::ai::specific::kikotsoka::MCTSPlayer(
      openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
      engine, 1000, false);
//  openxum::core::common::Player *player_two = new openxum::ai::specific::kikotsoka::RandomPlayer(
//      openxum::core::games::kikotsoka::Color::WHITE, openxum::core::games::kikotsoka::Color::BLACK,
//      engine);
  openxum::core::common::Player *current_player = player_one;
  unsigned int possible_move_number = 0;
  unsigned int turn_number = 0;

  std::map<int, std::vector<int>> sizes;
  std::map<int, std::vector<double>> gains;
  std::vector<std::string> moves;
  std::map<int, std::vector<int>> levels;
  std::map<int, std::vector<std::string>> patterns;

  sizes[openxum::core::games::kikotsoka::Color::BLACK] = std::vector<int>();
  sizes[openxum::core::games::kikotsoka::Color::WHITE] = std::vector<int>();
  gains[openxum::core::games::kikotsoka::Color::BLACK] = std::vector<double>();
  gains[openxum::core::games::kikotsoka::Color::WHITE] = std::vector<double>();
  patterns[openxum::core::games::kikotsoka::Color::BLACK] = std::vector<std::string>();
  patterns[openxum::core::games::kikotsoka::Color::WHITE] = std::vector<std::string>();

  while (not engine->is_finished()) {
    openxum::core::common::Move *move = current_player->get_move();
    int color = engine->current_color();

    possible_move_number += engine->get_possible_move_list().size();
    sizes[engine->current_color()].push_back(engine->get_possible_move_list().size());
    moves.push_back(move->encode());

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

      gains[engine->current_color()].push_back(engine->gain(engine->current_color()));
      gains[opponent_color].push_back(engine->gain(opponent_color));
      levels[openxum::core::games::kikotsoka::Color::BLACK].push_back(engine->_black_level);
      levels[openxum::core::games::kikotsoka::Color::WHITE].push_back(engine->_white_level);
      ++turn_number;
    }
    delete move;
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

    output_file << "T;" << a << ";" << b << ";"
                << engine->move_number() << ";" << possible_move_number << ";"
                << turn_number << ";"
                << engine->_black_level << ";" << engine->_white_level << ";"
                << engine->_black_captured_piece_number << ";"
                << engine->_white_captured_piece_number << ";"
                << (engine->winner_is() == openxum::core::games::kikotsoka::Color::BLACK ? "b"
                                                                                         : "w")
                << std::endl;
  }

  delete player_one;
  delete player_two;
  delete engine;
}

void test_random()
{
  unsigned int max = std::thread::hardware_concurrency();
  ThreadPool pool(max);
  std::vector<std::future<void> > results;

  for (unsigned int a = 12; a < 13; ++a) {
    for (unsigned int b = 42; b < 43; ++b) {
      for (unsigned int i = 0; i < 20; ++i) {
        results.emplace_back(pool.enqueue([=] { play(a, b, 5); }));
      }
    }
  }
  for (auto &&result: results) {
    result.get();
  }
}

int main(int, const char **)
{
//    test_mcts();
//    test_random();

  unsigned int max = std::thread::hardware_concurrency();
  ThreadPool pool(max);
  std::vector<std::future<void> > results;

  for (int i = 0; i < 1000; ++i) {
    results.emplace_back(pool.enqueue([=] { play(12, 42, 5); }));
  }
  for (auto &&result: results) {
    result.get();
  }

  output_file.close();
  return EXIT_SUCCESS;
}