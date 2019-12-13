/**
 * @file openxum/core/games/kamisado/engine.cpp
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

#include <openxum/core/games/kamisado/engine.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kamisado {

static TowerColor::Values initial_colors[8][8] = {
    {TowerColor::ORANGE, TowerColor::BLUE, TowerColor::PURPLE, TowerColor::PINK, TowerColor::YELLOW,
     TowerColor::RED, TowerColor::GREEN, TowerColor::BROWN},
    {TowerColor::RED, TowerColor::ORANGE, TowerColor::PINK, TowerColor::GREEN, TowerColor::BLUE,
     TowerColor::YELLOW, TowerColor::BROWN, TowerColor::PURPLE},
    {TowerColor::GREEN, TowerColor::PINK, TowerColor::ORANGE, TowerColor::RED, TowerColor::PURPLE,
     TowerColor::BROWN, TowerColor::YELLOW, TowerColor::BLUE},
    {TowerColor::PINK, TowerColor::PURPLE, TowerColor::BLUE, TowerColor::ORANGE, TowerColor::BROWN,
     TowerColor::GREEN, TowerColor::RED, TowerColor::YELLOW},
    {TowerColor::YELLOW, TowerColor::RED, TowerColor::GREEN, TowerColor::BROWN, TowerColor::ORANGE,
     TowerColor::BLUE, TowerColor::PURPLE, TowerColor::PINK},
    {TowerColor::BLUE, TowerColor::YELLOW, TowerColor::BROWN, TowerColor::PURPLE, TowerColor::RED,
     TowerColor::ORANGE, TowerColor::PINK, TowerColor::GREEN},
    {TowerColor::PURPLE, TowerColor::BROWN, TowerColor::YELLOW, TowerColor::BLUE, TowerColor::GREEN,
     TowerColor::PINK, TowerColor::ORANGE, TowerColor::RED},
    {TowerColor::BROWN, TowerColor::GREEN, TowerColor::RED, TowerColor::YELLOW, TowerColor::PINK,
     TowerColor::PURPLE, TowerColor::BLUE, TowerColor::ORANGE}
};

std::string Engine::GAME_NAME = "kamisado";

// constructors
Engine::Engine(int type, int color)
    : _color(color), _type(type)
{
  for (int i = 0; i < 8; i++) {
    _black_towers.emplace_back(State(i, 0, initial_colors[i][0]));
  }
  for (int i = 0; i < 8; i++) {
    _white_towers.emplace_back(State(i, 7, initial_colors[i][7]));
  }
  _phase = MOVE_TOWER;
  _play_color = TowerColor::NONE;
}

Engine::~Engine()
{}

// public methods
Engine *Engine::clone() const
{
  auto e = new Engine(_type, _color);

  e->_black_towers.clear();
  e->_white_towers.clear();
  for (const State &s: _black_towers) {
    e->_black_towers.push_back(s);
  }
  for (const State &s: _white_towers) {
    e->_white_towers.push_back(s);
  }
  e->_phase = _phase;
  e->_play_color = _play_color;
  return e;
}

int Engine::current_color() const
{
  return _color;
}

openxum::core::common::Moves<Decision> Engine::get_possible_move_list() const
{
  openxum::core::common::Moves<Decision> moves;
  const Coordinates &playable_tower = find_playable_tower(_color);

  if (playable_tower.is_valid()) {
    const std::vector<Coordinates> &list = get_possible_moving_list(
        State(playable_tower.x(), playable_tower.y(), _play_color), _color);

    for (const Coordinates &c: list) {
      moves.push_back(common::Move<Decision>(Decision(MOVE, playable_tower, c)));
    }
  } else {
    if (_play_color == TowerColor::NONE) {
      const std::vector<State> &towers = _color == Color::BLACK ? _black_towers : _white_towers;

      for (const State &s: towers) {
        const std::vector<Coordinates> &list = get_possible_moving_list(
            State(s.x(), s.y(), _play_color), _color);

        for (const Coordinates &c: list) {
          moves.push_back(common::Move<Decision>(Decision(MOVE, s.coordinates(), c)));
        }
      }
    } else {
      moves.push_back(common::Move<Decision>(Decision(PASS, Coordinates(), Coordinates())));
    }
  }
  return moves;
}

std::string Engine::id() const
{
  std::string str;

  for (const State &s: _black_towers) {
    str += s.id();
  }
  for (const State &s: _white_towers) {
    str += s.id();
  }
  return str;
}

bool Engine::is_finished() const
{
  return _phase == FINISH;
}

void Engine::move(const openxum::core::common::Move<Decision> &move)
{
  std::for_each(move.begin(), move.end(), [this](const Decision &m) {
    if (m.type() == MOVE) {
      move_tower(m.from(), m.to());
    } else {
      change_color();
    }
  });
}

std::string Engine::to_string() const
{
  std::string str;

  str += "BLACK: ";
  for (const State &s: _black_towers) {
    str += s.to_string() + " ";
  }
  str += " / WHITE: ";
  for (const State &s: _white_towers) {
    str += s.to_string() + " ";
  }
  return str;
}

int Engine::winner_is() const
{
  if (is_finished()) {
    return _color;
  } else {
    return NONE;
  }
}

// private methods
void Engine::change_color()
{
  _color = next_color(_color);
}

Coordinates Engine::find_playable_tower(int color) const
{
  Coordinates result;

  if (_play_color != TowerColor::NONE) {
    const std::vector<State> &towers = color == Color::BLACK ? _black_towers : _white_towers;

    for (const State &s : towers) {
      if (s.color() == _play_color) {
        result = s.coordinates();
      }
    }
  }
  return result;
}

State &Engine::find_towers2(const Coordinates &coordinates, int color)
{
  std::vector<State> &towers = color == Color::BLACK ? _black_towers : _white_towers;
  bool found = false;
  int i = 0;

  while (i < 8 and not found) {
    if (towers[i].x() == coordinates.x() and towers[i].y() == coordinates.y()) {
      found = true;
    } else {
      i++;
    }
  }
  return towers[i];
}

std::vector<Coordinates> Engine::get_possible_moving_list(const State &tower, int color) const
{
  std::vector<Coordinates> moves;
  int step = color == Color::BLACK ? 1 : -1;
  int limit = color == Color::BLACK ? 8 : -1;

  //column
  {
    int line = tower.y() + step;
    Coordinates coordinates(tower.x(), line);

    while (line != limit and is_empty(coordinates)) {
      moves.push_back(coordinates);
      line += step;
      coordinates = Coordinates(tower.x(), line);
    }
  }

  // right diagonal
  {
    int column = tower.x() + 1;
    int line = tower.y() + step;
    Coordinates coordinates(column, line);

    while (line != limit && column != 8 and is_empty(coordinates)) {
      moves.push_back(coordinates);
      line += step;
      ++column;
      coordinates = Coordinates(column, line);
    }
  }

  // left diagonal
  {
    int column = tower.x() - 1;
    int line = tower.y() + step;
    Coordinates coordinates(column, line);

    while (line != limit && column != -1 and is_empty(coordinates)) {
      moves.push_back(coordinates);
      line += step;
      --column;
      coordinates = Coordinates(column, line);
    }
  }
  return moves;
}

bool Engine::is_empty(const Coordinates &coordinates) const
{
  bool found = false;
  int i = 0;

  while (i < 8 and not found) {
    if (_black_towers[i].x() == coordinates.x() and _black_towers[i].y() == coordinates.y()) {
      found = true;
    } else {
      i++;
    }
  }
  i = 0;
  while (i < 8 and not found) {
    if (_white_towers[i].x() == coordinates.x() and _white_towers[i].y() == coordinates.y()) {
      found = true;
    } else {
      i++;
    }
  }
  return not found;
}

void Engine::move_tower(const Coordinates &selected_tower, const Coordinates &selected_cell)
{
  State &tower = find_towers2(selected_tower, _color);

  if (tower.x() != -1) {
    tower = selected_cell;
  }
  if ((_color == BLACK and tower.y() == 7) or (_color == WHITE and tower.y() == 0)) {
    _phase = FINISH;
  } else {
    _play_color = initial_colors[tower.x()][tower.y()];
    if (not pass(next_color(_color))) {
      change_color();
    } else {
      const Coordinates &playable_tower = find_playable_tower(next_color(_color));

      _play_color = initial_colors[playable_tower.x()][playable_tower.y()];
      if (pass(_color)) {
        _phase = FINISH;
        change_color();
      }
    }
  }
}

int Engine::next_color(int color)
{
  return color == WHITE ? BLACK : WHITE;
}

bool Engine::pass(int color) const
{
  const Coordinates &playable_tower = find_playable_tower(color);

  if (playable_tower.is_valid()) {
    const std::vector<Coordinates> &list = get_possible_moving_list(
        State(playable_tower.x(), playable_tower.y(), _play_color), color);

    return list.empty();
  } else {
    return false;
  }
}
}
}
}
}