/**
 * @file test/main_kamisaod.cpp
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

#include <openxum/core/games/kamisado/engine.hpp>
#include <openxum/core/games/kamisado/game_type.hpp>
#include <openxum/ai/specific/kamisado/mcts_player.hpp>
#include <openxum/ai/specific/kamisado/random_player.hpp>

std::ofstream output_file("result.csv");
std::mutex mutex;

void play()
{
  openxum::core::games::kamisado::Engine *engine = new openxum::core::games::kamisado::Engine(
      openxum::core::games::kamisado::SIMPLE,
      openxum::core::games::kamisado::Color::BLACK);
  openxum::core::common::Player<openxum::core::games::kamisado::Decision>
      *player_one = new openxum::ai::specific::kamisado::MCTSPlayer(
  openxum::core::games::kamisado::Color::BLACK, openxum::core::games::kamisado::Color::WHITE,
      engine);
  openxum::core::common::Player<openxum::core::games::kamisado::Decision>
      *player_two = new openxum::ai::specific::kamisado::MCTSPlayer(
      openxum::core::games::kamisado::Color::WHITE, openxum::core::games::kamisado::Color::BLACK,
      engine);
  openxum::core::common::Player<openxum::core::games::kamisado::Decision>
      *current_player = player_one;
  unsigned int possible_move_number = 0;
  unsigned int turn_number = 0;

  std::map<int, std::vector<int>> sizes;
  std::map<int, std::vector<double>> gains;
  std::vector<std::string> moves;
  std::map<int, std::vector<double>> distances;
  std::map<int, std::vector<unsigned int>> distance_evaluations;

  sizes[openxum::core::games::kamisado::Color::BLACK] = std::vector<int>();
  sizes[openxum::core::games::kamisado::Color::WHITE] = std::vector<int>();
  gains[openxum::core::games::kamisado::Color::BLACK] = std::vector<double>();
  gains[openxum::core::games::kamisado::Color::WHITE] = std::vector<double>();

  while (not engine->is_finished()) {
    openxum::core::common::Move<openxum::core::games::kamisado::Decision>
        move = current_player->get_move();
    int color = engine->current_color();

    possible_move_number += engine->get_possible_move_list().size();
    sizes[engine->current_color()].push_back(engine->get_possible_move_list().size());
    moves.push_back(move.encode());

//    std::cout << turn_number << ": " << move.to_string() << std::endl;

    engine->move(move);

    if (engine->current_color() == player_one->color()) {
      current_player = player_one;
    } else {
      current_player = player_two;
    }
    if (color != engine->current_color()) {
      int opponent_color = engine->current_color() == openxum::core::games::kamisado::Color::BLACK
                           ? openxum::core::games::kamisado::Color::WHITE
                           : openxum::core::games::kamisado::Color::BLACK;

      gains[engine->current_color()].push_back(engine->gain(engine->current_color(), false));
      gains[opponent_color].push_back(engine->gain(opponent_color, false));
      if (color == openxum::core::games::kamisado::Color::BLACK) {
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
    for (int e: sizes[openxum::core::games::kamisado::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "S[WHITE];";
    for (int e: sizes[openxum::core::games::kamisado::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "G[BLACK];";
    for (int e: gains[openxum::core::games::kamisado::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "G[WHITE];";
    for (int e: gains[openxum::core::games::kamisado::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "D[BLACK];";
    for (int e: distances[openxum::core::games::kamisado::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "D[WHITE];";
    for (int e: distances[openxum::core::games::kamisado::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "E[BLACK];";
    for (int e: distance_evaluations[openxum::core::games::kamisado::Color::BLACK]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "E[WHITE];";
    for (int e: distance_evaluations[openxum::core::games::kamisado::Color::WHITE]) {
      output_file << e << ";";
    }
    output_file << std::endl;
    output_file << "X;";
    for (const std::string &m: moves) {
      output_file << m << ";";
    }
    output_file << std::endl;

    output_file << "T;" << engine->move_number() << ";" << possible_move_number << ";"
                << turn_number << ";"
                << (engine->winner_is() == openxum::core::games::kamisado::Color::BLACK ? "b"
                                                                                        : "w")
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

  for (int i = 0; i < 10; ++i) {
    results.emplace_back(pool.enqueue([=] { play(); }));
  }

  for (auto &&result: results) {
    result.get();
  }

  output_file.close();
  return EXIT_SUCCESS;
}