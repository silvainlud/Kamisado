/**
 * @file openxum/ai/common/random_player.hpp
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

#ifndef OPENXUM_AI_COMMON_RANDOM_PLAYER_HPP
#define OPENXUM_AI_COMMON_RANDOM_PLAYER_HPP

#include <random>
#include <openxum/core/common/player.hpp>
#include <openxum/core/common/engine.hpp>

namespace openxum {
namespace ai {
namespace common {

template <class Decision>
class RandomPlayer : public openxum::core::common::Player<Decision>
{
public:
  RandomPlayer(int c, int o, openxum::core::common::Engine<Decision> *e)
      : openxum::core::common::Player<Decision>(c, o, e), _rng(_random_device())
  {}

  openxum::core::common::Move<Decision> get_move() override
  {
    const openxum::core::common::Moves<Decision> &list = this->engine().get_possible_move_list();
    openxum::core::common::Move<Decision> move;

    if (not list.empty()) {
      std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);

      move = list[distribution(_rng)];
      compute_next_goal_distance(move);
    }
    return move;
  }

  double get_next_goal_distance() override
  { return _next_goal_distance; }

private:
  void compute_next_goal_distance(const openxum::core::common::Move<Decision> &move)
  {
    openxum::core::common::Engine<Decision> *e = this->engine().clone();

    e->move(move);

    unsigned int initial_goal = e->current_goal(this->color());

    _next_goal_distance = 0;
    while (not e->is_finished() and e->current_goal(this->color()) == initial_goal) {
      const openxum::core::common::Moves<Decision> &list = e->get_possible_move_list();

      if (not list.empty()) {
        std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);

        e->move(list[distribution(_rng)]);
        ++_next_goal_distance;
      }
    }
    delete e;
  }

  std::random_device _random_device;
  std::mt19937 _rng;
  unsigned int _next_goal_distance;
};

}
}
}

#endif