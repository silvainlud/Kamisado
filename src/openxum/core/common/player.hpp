/**
 * @file openxum/core/common/player.hpp
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

#ifndef OPENXUM_CORE_COMMON_PLAYER_HPP
#define OPENXUM_CORE_COMMON_PLAYER_HPP

#include <openxum/core/common/engine.hpp>
#include <openxum/core/common/move.hpp>

namespace openxum {
namespace core {
namespace common {

template <class Decision>
class Player
{
public:
  Player(int c, int o, Engine<Decision> *e)
      : _color(c), _opponent_color(o), _engine(e)
  { _level = 0; }

  virtual ~Player() = default;

  int color() const
  { return _color; }

  const Engine<Decision> &engine() const
  { return *_engine; }

  Engine<Decision> &engine()
  { return *_engine; }

  virtual Move<Decision> get_move() = 0;

  virtual double get_next_goal_distance() = 0;

  void move(const Move<Decision> *move)
  { _engine->move(move); }

  int opponent_color() const
  { return _opponent_color; }

  void set_level(int l)
  { _level = l; }

private:
  int _color;
  int _opponent_color;
  Engine<Decision> *_engine;
  int _level;
};

}
}
}

#endif