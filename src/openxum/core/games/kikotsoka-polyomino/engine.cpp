/**
 * @file openxum/core/games/kikotsoka-polyomino/engine.cpp
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

#include <openxum/core/games/kikotsoka-polyomino/engine.hpp>
#include <openxum/core/games/kikotsoka-polyomino/move.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kikotsoka_polyomino {

Engine::Patterns Engine::PATTERNS = {
    {
        {{false, false, true}, {false, true, false}, {true, false, false}},
        {{true, false, false}, {false, true, false}, {false, false, true}}
    },
    {
        {{false, true, false}, {false, true, false}, {true, false, true}},
        {{true, false, false}, {false, true, true}, {true, false, false}},
        {{true, false, true}, {false, true, false}, {false, true, false}},
        {{false, false, true}, {true, true, false}, {false, false, true}}
    },
    {
        {{false, true, false}, {true, true, false}, {false, true, true}},
        {{false, true, false}, {true, true, true}, {true, false, false}},
        {{true, true, false}, {false, true, true}, {false, true, false}},
        {{false, false, true}, {true, true, true}, {false, true, false}}
    },
    {
        {{true, true, false}, {true, false, true}, {false, true, true}},
        {{false, true, true}, {true, false, true}, {true, true, false}}
    },
    {
        {{true, false, true}, {true, false, true}, {true, true, true}},
        {{true, true, true}, {true, false, false}, {true, true, true}},
        {{true, true, true}, {true, false, true}, {true, false, true}},
        {{true, true, true}, {false, false, true}, {true, true, true}}
    }
};

Engine::Configuration Engine::CONFIGURATIONS[1] = {{18, 24}};

std::string Engine::GAME_NAME = "kikotsoka-polyomino";

std::vector<std::vector<std::vector<int> > > Engine::BLACK_POINTS = {
    {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
    {{0, 0, 1}, {0, 0, 0}, {0, 0, 1}},
    {{1, 1, 1}, {0, 1, 0}, {0, 0, 0}},
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}},
    {{0, 0, 0}, {1, 0, 0}, {1, 1, 1}},
    {{1, 0, 0}, {1, 1, 0}, {1, 1, 1}},
    {{0, 1, 0}, {1, 0, 0}, {0, 1, 1}},
    {{0, 0, 1}, {1, 1, 0}, {0, 1, 0}},
    {{1, 0, 1}, {1, 0, 0}, {0, 1, 1}},
    {{0, 1, 1}, {1, 1, 1}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}},
    {{0, 0, 0}, {1, 1, 0}, {0, 1, 0}},
    {{1, 1, 1}, {1, 1, 1}, {0, 1, 0}},
    {{0, 0, 1}, {1, 1, 1}, {0, 0, 1}},
    {{0, 1, 0}, {0, 0, 0}, {1, 1, 1}},
    {{1, 1, 1}, {0, 1, 1}, {1, 0, 1}},
    {{1, 0, 0}, {1, 0, 0}, {1, 0, 1}},
    {{1, 0, 0}, {0, 0, 1}, {0, 1, 1}},
    {{1, 1, 1}, {0, 0, 1}, {0, 1, 1}},
    {{1, 1, 0}, {1, 0, 1}, {1, 1, 1}},
    {{1, 1, 0}, {0, 0, 0}, {1, 1, 0}},
    {{1, 1, 0}, {0, 1, 0}, {0, 1, 1}},
    {{0, 1, 1}, {1, 0, 0}, {1, 0, 0}},
    {{0, 1, 1}, {1, 0, 0}, {0, 1, 1}}};

std::vector<std::vector<std::vector<int> > > Engine::WHITE_POINTS = {
    {{0, 1, 0}, {0, 0, 0}, {0, 0, 1}},
    {{0, 1, 0}, {1, 1, 0}, {0, 0, 1}},
    {{1, 0, 1}, {0, 1, 1}, {0, 0, 0}},
    {{0, 0, 0}, {1, 1, 1}, {1, 1, 0}},
    {{0, 0, 1}, {0, 1, 1}, {0, 1, 0}},
    {{0, 0, 0}, {0, 1, 0}, {0, 1, 1}},
    {{0, 1, 1}, {1, 0, 0}, {1, 1, 1}},
    {{0, 0, 1}, {0, 1, 0}, {0, 1, 0}},
    {{1, 1, 0}, {0, 1, 0}, {0, 0, 1}},
    {{1, 1, 0}, {0, 0, 1}, {0, 1, 1}},
    {{1, 1, 0}, {0, 1, 1}, {0, 1, 1}},
    {{1, 0, 1}, {1, 1, 1}, {0, 0, 0}},
    {{0, 1, 0}, {1, 0, 1}, {1, 0, 0}},
    {{1, 1, 0}, {0, 1, 0}, {1, 0, 1}},
    {{0, 1, 1}, {0, 0, 1}, {0, 1, 0}},
    {{1, 0, 1}, {1, 0, 1}, {1, 0, 1}},
    {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}},
    {{0, 1, 1}, {0, 1, 1}, {1, 1, 0}},
    {{0, 0, 1}, {1, 1, 0}, {0, 0, 0}},
    {{1, 1, 0}, {0, 1, 0}, {0, 1, 1}},
    {{1, 0, 1}, {0, 0, 1}, {1, 0, 1}},
    {{1, 0, 0}, {1, 0, 1}, {1, 0, 1}},
    {{1, 0, 0}, {1, 1, 1}, {1, 1, 0}},
    {{0, 1, 1}, {0, 0, 1}, {1, 0, 1}}};
std::vector<int>
    Engine::BLACK_INDEXES = {7, 4, 9, 1, 2, 8, 9, 1, 5, 4, 11, 10, 6, 13, 3, 1, 9, 9, 12,
                             4, 13, 0, 1, 9};
std::vector<int>
    Engine::WHITE_INDEXES = {3, 9, 5, 10, 7, 6, 7, 0, 2, 12, 11, 4, 8, 9, 1, 8, 9, 7, 5,
                             13, 9, 8, 9, 6};

typedef State::Values *PtrState;

// constructors
Engine::Engine(int type, int color)
    : _color(Color(color)), _type(type)
{
  _size = CONFIGURATIONS[_type].size;
  _board = new PtrState[_size];
  for (int l = 0; l < _size; ++l) {
    _board[l] = new State::Values[_size];
    for (int c = 0; c < _size; ++c) {
      _board[l][c] = State::VACANT;
    }
  }
  init_polyominos(Color::BLACK, _black_polyominos, CONFIGURATIONS[_type].polyomino_number);
  init_polyominos(Color::WHITE, _white_polyominos, CONFIGURATIONS[_type].polyomino_number);
  _black_polyomino_number = CONFIGURATIONS[_type].polyomino_number;
  _white_polyomino_number = CONFIGURATIONS[_type].polyomino_number;
  _pass = 0;
  _black_captured_piece_number = 0;
  _white_captured_piece_number = 0;
  _black_level = 0;
  _white_level = 0;
  _previous_black_level = 0;
  _previous_white_level = 0;
  _black_failed = false;
  _white_failed = false;
  _phase = Phase::PUT_POLYOMINO;
}

Engine::~Engine()
{
  for (int l = 0; l < _size; ++l) {
    delete[] _board[l];
  }
  delete[] _board;
  for (size_t k = 0; k < _black_polyominos.size(); ++k) {
    delete _black_polyominos[k];
  }
  for (size_t k = 0; k < _white_polyominos.size(); ++k) {
    delete _white_polyominos[k];
  }
}

// public methods
int Engine::best_is() const
{
  if (_black_level == 6 or _black_level > _white_level or _white_failed) {
    return Color::BLACK;
  } else if (_white_level == 6 or _black_level < _white_level or _black_failed) {
    return Color::WHITE;
  } else {
    if (_black_captured_piece_number > _white_captured_piece_number) {
      return Color::BLACK;
    } else if (_black_captured_piece_number < _white_captured_piece_number) {
      return Color::WHITE;
    } else {
      return Color::NONE;
    }
  }
}

Engine *Engine::clone() const
{
  auto e = new Engine;

  e->_type = _type;
  e->_color = _color;
  e->_board = new PtrState[_size];
  for (int l = 0; l < _size; ++l) {
    e->_board[l] = new State::Values[_size];
  }
  e->_move_number = _move_number;
  e->_size = _size;
  for (int l = 0; l < _size; ++l) {
    for (int c = 0; c < _size; ++c) {
      e->_board[l][c] = _board[l][c];
    }
  }
  for (size_t k = 0; k < _black_polyominos.size(); ++k) {
    e->_black_polyominos.push_back(_black_polyominos[k]->clone());
  }
  for (size_t k = 0; k < _white_polyominos.size(); ++k) {
    e->_white_polyominos.push_back(_white_polyominos[k]->clone());
  }
  e->_black_polyomino_number = _black_polyomino_number;
  e->_white_polyomino_number = _white_polyomino_number;
  e->_last_coordinates = _last_coordinates;
  e->_pass = _pass;
  e->_black_captured_piece_number = _black_captured_piece_number;
  e->_white_captured_piece_number = _white_captured_piece_number;
  e->_black_level = _black_level;
  e->_white_level = _white_level;
  e->_previous_black_level = _previous_black_level;
  e->_previous_white_level = _previous_white_level;
  e->_black_failed = _black_failed;
  e->_white_failed = _white_failed;
  e->_phase = _phase;
  return e;
}

double Engine::gain(int color) const
{
  double g;

  if (winner_is() == Color::BLACK) {
    if (_black_level == _white_level) {
      g = (_black_captured_piece_number - _white_captured_piece_number) / 2.;
    } else {
      g = 2 * (_black_level - _white_level);
    }
  } else if (winner_is() == Color::WHITE) {
    if (_black_level == _white_level) {
      g = (_white_captured_piece_number - _black_captured_piece_number) / 2.;
    } else {
      g = 2 * (_white_level - _black_level);
    }
  } else {
    if (_black_level == _white_level) {
      g = (_black_captured_piece_number - _white_captured_piece_number) / 2.;
    } else {
      g = 2 * (_black_level - _white_level);
    }
  }
  return color == Color::BLACK ? g : -g;
}

openxum::core::common::Moves Engine::get_possible_move_list() const
{
  openxum::core::common::Moves moves;

  if (_phase == Phase::PUT_POLYOMINO) {
    Engine *e = clone();
    bool first =
        _color == Color::BLACK ? _black_polyomino_number == (int) _black_polyominos.size() :
        _white_polyomino_number == (int) _white_polyominos.size();
    const std::vector<Polyomino *> &polyominos =
        _color == Color::BLACK ? _black_polyominos : _white_polyominos;

    for (size_t k = 0; k < polyominos.size(); ++k) {
      if (polyominos[k]->is_free()) {
        Polyomino *polyomino = polyominos[k]->clone();

        for (int r = 0; r < (int) Polyomino::POLYOMINOS[polyomino->shape_index()].size(); ++r) {
          for (int l = 0; l < _size; ++l) {
            for (int c = 0; c < _size; ++c) {
              if (e->is_valid(polyomino, Coordinates(c, l), first)) {
                moves.push_back(
                    new Move(MoveType::PUT_POLYOMINO, _color, Coordinates(c, l), k,
                             r, -1));
              }
            }
          }
          polyomino->rotate();
        }
        delete polyomino;
      }
    }
    if (moves.empty()) {
      moves.push_back(new Move(MoveType::PASS, _color, Coordinates(), -1, -1, -1));
    }
    delete e;
  } else { // phase = Phase::CHOICE_PATTERN
    std::vector<Coordinates> result = check_patterns(_last_coordinates);

    for (int i = 0; i < (int) result.size(); ++i) {
      moves.push_back(new Move(MoveType::CHOICE_PATTERN, _color, Coordinates(), -1, -1, i));
    }
  }
  return moves;
}

std::string Engine::id() const
{
  std::string str;

  for (int l = 0; l < _size; ++l) {
    for (int c = 0; c < _size; ++c) {
      str += std::to_string(_board[l][c]);
    }
  }

  for (size_t k = 0; k < _black_polyominos.size(); ++k) {
    if (_black_polyominos[k]->is_free()) {
      str += std::to_string(k);
    }
  }
  for (size_t k = 0; k < _white_polyominos.size(); ++k) {
    if (_white_polyominos[k]->is_free()) {
      str += std::to_string(k);
    }
  }
  str += std::to_string(_pass);
  str += std::to_string(_black_captured_piece_number);
  str += std::to_string(_white_captured_piece_number);
  str += std::to_string(_black_level);
  str += std::to_string(_white_level);
  str += std::to_string(_phase);
  return str;
}

bool Engine::is_finished() const
{
  return _black_level == 5 or _white_level == 5 or _pass == 2 or _black_failed or _white_failed or
      (_black_polyomino_number == 0 and _white_polyomino_number == 0);
}

bool Engine::is_stoppable() const
{
  return is_finished() or _previous_black_level != _black_level
      or _previous_white_level != _white_level;
}

void Engine::move(const openxum::core::common::Move *move)
{
  auto *m = dynamic_cast<const openxum::core::games::kikotsoka_polyomino::Move *>(move);

  ++_move_number;
  _previous_black_level = _black_level;
  _previous_white_level = _white_level;
  if (m != nullptr) {
    if (m->type() == MoveType::PUT_POLYOMINO) {
      Polyomino *polyomino =
          m->color() == Color::BLACK ? _black_polyominos[m->polyomino_index()] :
          _white_polyominos[m->polyomino_index()];

      for (int i = polyomino->rotation(); i < m->rotation(); ++i) {
        polyomino->rotate();
      }
      put_polyomino(polyomino, m->to());
      _last_coordinates = m->to();

      std::vector<Coordinates> result = check_patterns(_last_coordinates);

      if (not result.empty()) {
        if (result.size() == 1) {
          capture(result[0]);
          block(result[0]);
          if (_color == Color::BLACK) {
            ++_black_level;
          } else {
            ++_white_level;
          }
          change_color();
          _phase = Phase::PUT_POLYOMINO;
        } else {
          _phase = Phase::CHOICE_PATTERN;
        }
      } else {
        change_color();
        _phase = Phase::PUT_POLYOMINO;
      }
      _pass = 0;
    } else if (m->type() == MoveType::CHOICE_PATTERN) {
      std::vector<Coordinates> result = check_patterns(_last_coordinates);

      capture(result[m->index()]);
      block(result[m->index()]);
      if (_color == Color::BLACK) {
        ++_black_level;
      } else {
        ++_white_level;
      }
      change_color();
      _phase = Phase::PUT_POLYOMINO;
    } else if (m->type() == MoveType::PASS) {
      ++_pass;
      _last_coordinates = Coordinates();
      change_color();
      _phase = Phase::PUT_POLYOMINO;
    }
  }
}

std::string Engine::to_string() const
{
  std::string str("   ");

  for (int c = 0; c < _size; ++c) {
    std::string column;

    column = char(c + 65);
    str += " " + column + " ";
  }
  str += "\n";
  for (int l = 0; l < _size; ++l) {
    if (l < 9) {
      str += std::to_string(l + 1) + "  ";
    } else {
      str += std::to_string(l + 1) + " ";
    }
    for (int c = 0; c < _size; ++c) {
      str += State::to_string(_board[l][c]);
    }
    str += "\n";
  }

  str += std::to_string(_black_level) + " " + std::to_string(_white_level) + " " +
      std::to_string(_black_captured_piece_number) + " " +
      std::to_string(_white_captured_piece_number) + " " +
      std::to_string(_black_polyomino_number) + " " + std::to_string(_white_polyomino_number)
      + " " +
      std::to_string(_black_failed) + " " + std::to_string(_white_failed) + " "
      + std::to_string(_pass);

  return str;
}

int Engine::winner_is() const
{
  if (is_finished()) {
    if (_black_level == 5 or _black_level > _white_level or _white_failed) {
      return Color::BLACK;
    } else if (_white_level == 5 or _black_level < _white_level or _black_failed) {
      return Color::WHITE;
    } else {
      if (_black_captured_piece_number > _white_captured_piece_number) {
        return Color::BLACK;
      } else if (_black_captured_piece_number < _white_captured_piece_number) {
        return Color::WHITE;
      } else {
        return Color::NONE;
      }
    }
  } else {
    return Color::NONE;
  }
}

// private methods
void Engine::block(const Coordinates &origin)
{
  int l = origin.line();
  int c = origin.column();

  while (l < origin.line() + 3) {
    block_coordinates(Coordinates(c, l));
    ++c;
    if (c == origin.column() + 3) {
      c = origin.column();
      ++l;
    }
  }
}

void Engine::block_coordinates(const Coordinates &coordinates)
{
  const State::Values &old_value = _board[coordinates.line()][coordinates.column()];
  State::Values new_value =
      _color == Color::BLACK ? State::BLOCKED_IN_BLACK : State::BLOCKED_IN_WHITE;

  if (old_value == State::BLACK and _color == Color::BLACK) {
    new_value = State::BLACK_BLOCKED;
  } else if (old_value == State::WHITE and _color == Color::WHITE) {
    new_value = State::WHITE_BLOCKED;
  }
  _board[coordinates.line()][coordinates.column()] = new_value;
}

void Engine::capture(const Coordinates &origin)
{
  int l = origin.line();
  int c = origin.column();
  int n = 0;

  while (l < origin.line() + 3) {
    if (_board[l][c] == State::BLACK and _color == Color::WHITE) {
      _board[l][c] = State::VACANT;
      ++n;
    }
    if (_board[l][c] == State::WHITE and _color == Color::BLACK) {
      _board[l][c] = State::VACANT;
      ++n;
    }
    ++c;
    if (c == origin.column() + 3) {
      c = origin.column();
      ++l;
    }
  }
  if (_color == Color::BLACK) {
    _black_captured_piece_number += n;
  } else {
    _white_captured_piece_number += n;
  }
}

bool Engine::check_no_blocked(const Coordinates &origin) const
{
  int l = origin.line();
  int c = origin.column();
  bool blocked = false;

  while (not blocked and l < origin.line() + 3) {
    blocked = _board[l][c] == State::BLOCKED_IN_BLACK or _board[l][c] == State::BLOCKED_IN_WHITE or
        _board[l][c] == State::BLACK_BLOCKED or _board[l][c] == State::WHITE_BLOCKED;
    ++c;
    if (c == origin.column() + 3) {
      c = origin.column();
      ++l;
    }
  }
  return not blocked;
}

std::vector<Coordinates> Engine::check_pattern(const Pattern &pattern,
                                               const Coordinates &coordinates) const
{
  int l_min = coordinates.line() - 3 <= 0 ? 0 : coordinates.line() - 3;
  int c_min = coordinates.column() - 3 <= 0 ? 0 : coordinates.column() - 3;
  int l_max = coordinates.line() + 3 >= _size - 2 ? _size - 2 : coordinates.line() + 3;
  int c_max = coordinates.column() + 3 >= _size - 2 ? _size - 2 : coordinates.column() + 3;
  int l = l_min;
  int c = c_min;
  std::vector<Coordinates> origins;

  while (l < l_max) {
    if (check_no_blocked(Coordinates(c, l))) {
      if (check_pattern_in_block(Coordinates(c, l), pattern)) {
        origins.emplace_back(Coordinates(c, l));
      }
    }
    ++c;
    if (c == c_max) {
      c = c_min;
      ++l;
    }
  }
  return origins;
}

bool Engine::check_pattern_in_block(const Coordinates &origin, const Pattern &pattern) const
{
  int l = origin.line();
  int c = origin.column();
  bool ok = true;

  while (ok and l < origin.line() + 3) {
    bool value = pattern[l - origin.line()][c - origin.column()];

    if ((value == 0 and
        _board[l][c] != (_color == Color::BLACK ? State::BLACK : State::WHITE)) or
        (value == 1 and
            (_board[l][c] == (_color == Color::BLACK ? State::BLACK : State::WHITE)))) {
      ++c;
      if (c == origin.column() + 3) {
        c = origin.column();
        ++l;
      }
    } else {
      ok = false;
    }
  }
  return ok;
}

std::vector<Coordinates> Engine::check_patterns(const Coordinates &coordinates) const
{
  int level = 0;
  bool found = false;
  std::vector<Coordinates> origins;

  while (!found and level < 5) {
    const LevelPattern &pattern = PATTERNS[level];

    for (const Pattern &p: pattern) {
      const std::vector<Coordinates> &new_origins = check_pattern(p, coordinates);

      if (not new_origins.empty()) {
        origins.insert(
            origins.end(),
            std::make_move_iterator(new_origins.begin()),
            std::make_move_iterator(new_origins.end())
        );
        found = true;
      }
    }
    if (!found) {
      ++level;
    }
  }
  if (found) {
    int current_level = _color == Color::BLACK ? _black_level : _white_level;

    if (level == current_level) {
      return origins;
    } else {
      throw Failed();
    }
  } else {
    return std::vector<Coordinates>();
  }
}

void Engine::init_polyominos(const Color &color, std::vector<Polyomino *> &polyominos, int size)
{
  if (color == Color::BLACK) {
    for (int k = 0; k < size; ++k) {
      int points[3][3];

      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          points[i][j] = BLACK_POINTS[k][i][j];
        }
      }
      polyominos.push_back(new Polyomino(BLACK_INDEXES[k], points));
    }
  } else {
    for (int k = 0; k < size; ++k) {
      int points[3][3];

      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          points[i][j] = WHITE_POINTS[k][i][j];
        }
      }
      polyominos.push_back(new Polyomino(WHITE_INDEXES[k], points));
    }
  }
}

bool Engine::is_neighbour(int i, int j) const
{
  return i >= 0 and j >= 0 and i < _size and j < _size and
      (_board[i][j] == (_color == Color::BLACK ? State::BLACK_NO_VACANT : State::WHITE_NO_VACANT)
          or
              _board[i][j] == (_color == Color::BLACK ? State::BLACK_BLOCKED
                                                      : State::WHITE_BLOCKED) or
          _board[i][j] == (_color == Color::BLACK ? State::BLACK : State::WHITE));
}

bool Engine::is_valid(Polyomino *polyomino, const Coordinates &coordinates, bool first)
{
  const Polyomino::Matrix_3_3 &shape = polyomino->shape();
  bool ok = true;
  int i = -1;
  int counter = 0;

  while (ok and i < 2) {
    int line = coordinates.line() + i;
    int j = -1;

    while (ok && j < 2) {
      int column = coordinates.column() + j;

      ok = (line >= 0 and column >= 0 and line < _size and column < _size) or
          (column < 0 and shape[i + 1][j + 1] == 0)
          or (line < 0 and shape[i + 1][j + 1] == 0) ||
          (column >= _size and shape[i + 1][j + 1] == 0)
              or (line >= _size and shape[i + 1][j + 1] == 0);
      if (ok) {
        if (line >= 0 and column >= 0 and line < _size and column < _size) {
          ok = _board[line][column] == State::VACANT or shape[i + 1][j + 1] == 0;
        }
      }
      if (ok and not first) {
        if (shape[i + 1][j + 1] != 0) {
          for (int p = -1; p < 2; ++p) {
            for (int r = -1; r < 2; ++r) {
              if (is_neighbour(line + p, column + r)) {
                ++counter;
              }
            }
          }
        }
      }
      ++j;
    }
    ++i;
  }
  ok = ok and (first or (not first and counter > 0));
  if (ok && not first) {
    put_polyomino(polyomino, coordinates);
    try {
      check_patterns(coordinates);
    }
    catch (Failed &) {
      ok = false;
    }
    remove_polyomino(polyomino, coordinates);
  }
  return ok;
}

void Engine::put_polyomino(Polyomino *polyomino, const Coordinates &coordinates)
{
  const Polyomino::Matrix_3_3 &shape = polyomino->shape();

  polyomino->put(coordinates);
  if (_color == Color::BLACK) {
    --_black_polyomino_number;
  } else {
    --_white_polyomino_number;
  }
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      if (shape[i + 1][j + 1] == 2) {
        _board[coordinates.line() + i][coordinates.column() + j] =
            _color == Color::BLACK ? State::BLACK : State::WHITE;
      } else if (shape[i + 1][j + 1] == 1) {
        _board[coordinates.line() + i][coordinates.column() + j] =
            _color == Color::BLACK ? State::BLACK_NO_VACANT : State::WHITE_NO_VACANT;
      }
    }
  }
}

void Engine::remove_polyomino(Polyomino *polyomino, const Coordinates &coordinates)
{
  const Polyomino::Matrix_3_3 &shape = polyomino->shape();

  polyomino->remove();
  if (_color == Color::BLACK) {
    ++_black_polyomino_number;
  } else {
    ++_white_polyomino_number;
  }
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      if (shape[i + 1][j + 1] == 1 or shape[i + 1][j + 1] == 2) {
        _board[coordinates.line() + i][coordinates.column() + j] = State::VACANT;
      }
    }
  }
}

}
}
}
}