/**
 * @file openxum/core/common/engine.hpp
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

#ifndef OPENXUM_CORE_COMMON_ENGINE_HPP
#define OPENXUM_CORE_COMMON_ENGINE_HPP

#include <openxum/core/common/move.hpp>

namespace openxum {
namespace core {
namespace common {

template<class Decision>
class Engine
{
public:
  Engine()
      : _move_number(0)
  {}

  virtual ~Engine() = default;

//  virtual void apply_moves(const Moves &moves)
//  {
//    for (auto m: moves) {
//      move(m);
//    }
//  }

  virtual int best_is() const = 0;

//  virtual Move<Decision> *build_move() const = 0;

  virtual Engine *clone() const = 0;

  virtual int current_color() const = 0;

  virtual unsigned int current_goal(int /* color */) const
  { return is_finished() ? 1 : 0; }

  virtual double gain(int color, bool finish) const = 0;

  virtual unsigned int get_goal_number() const
  { return 1; }

  virtual const std::string &get_name() const = 0;

  virtual Moves<Decision> get_possible_move_list() const = 0;

  virtual std::string id() const = 0;

  virtual bool is_finished() const = 0;

  virtual bool is_stoppable() const = 0;

  virtual void move(const Move<Decision> &move) = 0;

  unsigned int move_number() const
  { return _move_number; }

  virtual void parse(const std::string &str) = 0;

  virtual std::string to_string() const = 0;

  virtual int winner_is() const = 0;

protected:
  unsigned int _move_number;
};

}
}
}

#endif