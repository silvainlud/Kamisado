/**
 * @file openxum/core/games/kikotsoka/engine.cpp
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

#include <openxum/core/games/kikotsoka/engine.hpp>
#include <openxum/core/games/kikotsoka/decision.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kikotsoka {

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

Engine::Configuration Engine::CONFIGURATIONS[3] = {{11, 32, 5},
                                                   {13, 42, 5},
                                                   {15, 52, 5}};

std::string Engine::GAME_NAME = "kikotsoka";

typedef State::Values *PtrState;

// constructors
Engine::Engine(int type, int color, int variant)
    : _color(Color(color)), _type(type), _variant(variant)
{
  _size = CONFIGURATIONS[_type].size;
  _board = new PtrState[_size];
  for (int l = 0; l < _size; ++l) {
    _board[l] = new State::Values[_size];
    for (int c = 0; c < _size; ++c) {
      _board[l][c] = State::VACANT;
    }
  }
  _black_piece_number = CONFIGURATIONS[_type].piece_number;
  _white_piece_number = CONFIGURATIONS[_type].piece_number;
  _black_shido_number = CONFIGURATIONS[_type].shido_number;
  _white_shido_number = CONFIGURATIONS[_type].shido_number;
  _pass = 0;
  _black_captured_piece_number = 0;
  _white_captured_piece_number = 0;
  _black_captured_shido_number = 0;
  _white_captured_shido_number = 0;

  // rule variants 1 and 2
  _black_level = 0;
  _white_level = 0;
  _previous_black_level = 0;
  _previous_white_level = 0;

  _black_failed = false;
  _white_failed = false;

  if (_variant == 1) {
    _black_possible_shido = true;
    _white_possible_shido = true;
    _phase = Phase::PUT_SHIDO;
  } else if (_variant == 2) {
    _phase = Phase::PUT_INITIAL_SHIDO;
  } else if (_variant == 3) {
    _previous_black_pattern_number = 0;
    _previous_white_pattern_number = 0;
    _phase = Phase::PUT_SHIDO;
  }
}

Engine::~Engine()
{
  for (int l = 0; l < _size; ++l) {
    delete[] _board[l];
  }
  delete[] _board;
}

// public methods
int Engine::best_is() const
{
  if (gain(Color::BLACK, false) > gain(Color::WHITE, false)) {
    return Color::BLACK;
  } else if (gain(Color::BLACK, false) < gain(Color::WHITE, false)) {
    return Color::WHITE;
  } else {
    return Color::NONE;
  }
}

Engine *Engine::clone() const
{
  auto e = new Engine(_type, _color, _variant);

  e->_move_number = _move_number;
  e->_size = _size;
  for (int l = 0; l < _size; ++l) {
    for (int c = 0; c < _size; ++c) {
      e->_board[l][c] = _board[l][c];
    }
  }
  e->_black_piece_number = _black_piece_number;
  e->_white_piece_number = _white_piece_number;
  e->_black_shido_number = _black_shido_number;
  e->_white_shido_number = _white_shido_number;
  e->_last_coordinates = _last_coordinates;
  e->_pass = _pass;
  e->_black_captured_piece_number = _black_captured_piece_number;
  e->_white_captured_piece_number = _white_captured_piece_number;
  e->_black_captured_shido_number = _black_captured_shido_number;
  e->_white_captured_shido_number = _white_captured_shido_number;
  e->_black_level = _black_level;
  e->_white_level = _white_level;
  e->_previous_black_level = _previous_black_level;
  e->_previous_white_level = _previous_white_level;
  e->_black_failed = _black_failed;
  e->_white_failed = _white_failed;
  e->_phase = _phase;

  // rule variant 2
  e->_black_possible_shido = _black_possible_shido;
  e->_white_possible_shido = _white_possible_shido;

  // rule variant 3
  e->_black_pattern_number = _black_pattern_number;
  e->_white_piece_number = _white_piece_number;
  e->_previous_black_pattern_number = _previous_black_pattern_number;
  e->_previous_white_pattern_number = _previous_white_pattern_number;
  e->_black_patterns = _black_patterns;
  e->_white_patterns = _white_patterns;

  return e;
}

openxum::core::common::Moves<Decision> Engine::get_possible_move_list() const
{
  openxum::core::common::Moves<Decision> moves;

  if (_phase == Phase::PUT_SHIDO) {
    get_possible_put_shido(moves);
    if (moves.empty()) {
      moves.push_back(common::Move<Decision>(
          Decision(DecisionType::PASS, _color, Coordinates(), -1)));
    }
  } else if (_phase == Phase::PUT_PIECE) {
    get_possible_put_piece(moves, _color);
    if (moves.empty()) {
      moves.push_back(common::Move<Decision>(
          Decision(DecisionType::PASS, _color, Coordinates(), -1)));
    }
  } else if (_phase == Phase::CHOICE_PIECE) {
    get_possible_put_shido(moves, true);
    get_possible_put_piece(moves, _color, true);
  } else { // phase = Phase::CHOICE_PATTERN
    std::vector<Coordinates> result = check_patterns();

    for (std::vector<Coordinates>::size_type i = 0; i < result.size(); ++i) {
      moves.push_back(common::Move<Decision>(
          Decision(DecisionType::CHOICE_PATTERN, _color, Coordinates(), (int) i)));
    }
  }

  // rule variant 2
  if (_variant == 2) {
    if (_phase == Phase::PUT_INITIAL_SHIDO) {
      int p = std::floor(_size / 2);

      if (_color == Color::BLACK) {
        moves.push_back(common::Move<Decision>(
            Decision(DecisionType::PUT_INITIAL_SHIDO, _color, Coordinates(p, p - 2), -1)));
        moves.push_back(common::Move<Decision>(
            Decision(DecisionType::PUT_INITIAL_SHIDO, _color, Coordinates(p, p + 2), -1)));
      } else {
        moves.push_back(common::Move<Decision>(
            Decision(DecisionType::PUT_INITIAL_SHIDO, _color, Coordinates(p - 2, p), -1)));
        moves.push_back(common::Move<Decision>(
            Decision(DecisionType::PUT_INITIAL_SHIDO, _color, Coordinates(p + 2, p), -1)));
      }
    }
  }
  return moves;
}

void Engine::get_possible_put_piece(common::Moves<Decision> &moves,
                                    const Color &color,
                                    bool decision) const
{
  if ((color == Color::BLACK and _black_piece_number > 0) or
      (color == Color::WHITE and _white_piece_number > 0)) {
    if (_variant == 1 or _variant == 2) {
      std::vector<std::vector<Possible_pattern_results>> possible_patterns = is_possible_patterns();
      int possible_cases_number = count_possible_cases(possible_patterns);

      if (possible_cases_number > 0) {
        std::vector<Engine::Possible_pattern_results> one_piece_patterns = get_one_piece_pattern(
            possible_patterns);

        for (int l = 0; l < _size; ++l) {
          for (int c = 0; c < _size; ++c) {
            if (is_valid(Coordinates(c, l)) and is_connect(Coordinates(c, l))
                and not possible_forbidden_pattern(Coordinates(c, l),
                                                   one_piece_patterns)) {
              if (decision) {
                moves.push_back(common::Move<Decision>(
                    {Decision(DecisionType::CHOICE_PIECE, color, Coordinates(), 1),
                     Decision(DecisionType::PUT_PIECE, color, Coordinates(c, l), -1)}));
              } else {
                moves.push_back(common::Move<Decision>(
                    Decision(DecisionType::PUT_PIECE, color, Coordinates(c, l), -1)));
              }
            }
          }
        }
      }
    } else if (_variant == 3) {
      for (int l = 0; l < _size; ++l) {
        for (int c = 0; c < _size; ++c) {
          if (is_valid(Coordinates(c, l)) and is_connect(Coordinates(c, l))) {
            if (decision) {
              moves.push_back(common::Move<Decision>(
                  {Decision(DecisionType::CHOICE_PIECE, _color, Coordinates(), 1),
                   Decision(DecisionType::PUT_PIECE, _color, Coordinates(c, l), -1)}));
            } else {
              moves.push_back(common::Move<Decision>(
                  Decision(DecisionType::PUT_PIECE, _color, Coordinates(c, l), -1)));
            }
          }
        }
      }
    }
  }
}

void Engine::get_possible_put_shido(common::Moves<Decision> &moves, bool decision) const
{
  if ((_color == Color::BLACK and _black_shido_number > 0) or
      (_color == Color::WHITE and _white_shido_number > 0)) {
    for (int l = 0; l < _size; ++l) {
      for (int c = 0; c < _size; ++c) {
        if (is_valid(Coordinates(c, l))) {
          if (decision) {
            moves.push_back(common::Move<Decision>(
                {Decision(DecisionType::CHOICE_PIECE, _color, Coordinates(), 0),
                 Decision(DecisionType::PUT_SHIDO, _color, Coordinates(c, l), -1)}));
          } else {
            moves.push_back(common::Move<Decision>(
                Decision(DecisionType::PUT_SHIDO, _color, Coordinates(c, l), -1)));
          }
        }
      }
    }
  }
}

double Engine::gain(int color, bool finish) const
{
  if (_variant == 1 or _variant == 2) {
    if (color == Color::BLACK) {
      int level_gain = _black_level - _white_level;

      return _black_captured_piece_number + 5 * _black_captured_shido_number
          + (level_gain > 0 ? level_gain : 0);
    } else {
      int level_gain = _white_level - _black_level;

      return _white_captured_piece_number + 5 * _white_captured_shido_number
          + (level_gain > 0 ? level_gain : 0);
    }
  } else if (_variant == 3) {
    if (finish) {
      if (color == Color::BLACK) {
        int level_gain = std::accumulate(_black_patterns.begin(), _black_patterns.end(), 0) -
            std::accumulate(_white_patterns.begin(), _white_patterns.end(), 0);

        return (_black_captured_piece_number + 2 * _black_captured_shido_number) -
            (_white_captured_piece_number + 2 * _white_captured_shido_number) + level_gain;
      } else {
        int level_gain = std::accumulate(_white_patterns.begin(), _white_patterns.end(), 0) -
            std::accumulate(_black_patterns.begin(), _black_patterns.end(), 0);

        return (_white_captured_piece_number + 2 * _white_captured_shido_number) -
            (_black_captured_piece_number + 2 * _black_captured_shido_number) + level_gain;
      }
    } else {
      if (color == Color::BLACK) {
        int level_gain = std::accumulate(_black_patterns.begin(), _black_patterns.end(), 0);

        return _black_captured_piece_number + 2 * _black_captured_shido_number + level_gain;
      } else {
        int level_gain = std::accumulate(_white_patterns.begin(), _white_patterns.end(), 0);

        return _white_captured_piece_number + 2 * _white_captured_shido_number + level_gain;
      }
    }
  }
}

std::string Engine::id() const
{
  std::string str;

  for (int l = 0; l < _size; ++l) {
    for (int c = 0; c < _size; ++c) {
      str += std::to_string(_board[l][c]);
    }
  }
  str += std::to_string(_black_shido_number);
  str += std::to_string(_white_shido_number);
  str += std::to_string(_pass);
  str += std::to_string(_black_captured_piece_number);
  str += std::to_string(_white_captured_piece_number);
  str += std::to_string(_black_captured_shido_number);
  str += std::to_string(_white_captured_shido_number);
  str += std::to_string(_black_level);
  str += std::to_string(_white_level);
  str += std::to_string(_phase);

  // rule variant 2
  str += std::to_string(_black_possible_shido);
  str += std::to_string(_white_possible_shido);

  // rule variant 3
  // TODO

  return str;
}

bool Engine::is_finished() const
{
  if (_variant == 1 or _variant == 2) {
    return _black_level == 5 or _white_level == 5 or _pass == 2 or _black_failed or _white_failed or
        (_black_piece_number == 0 and _white_piece_number == 0 and _black_shido_number == 0
            and _white_shido_number == 0);
  } else if (_variant == 3) {
    return _pass == 2 or (_black_piece_number == 0 and _white_piece_number == 0
        and _black_shido_number == 0 and _white_shido_number == 0);
  }
}

bool Engine::is_stoppable() const
{
  if (_variant == 1 or _variant == 2) {
    return is_finished() or _previous_black_level != _black_level
        or _previous_white_level != _white_level;
  } else if (_variant == 3) {
    return is_finished() or _previous_black_pattern_number != _black_pattern_number
        or _previous_white_pattern_number != _white_pattern_number;
  }
}

void Engine::move(const openxum::core::common::Move<Decision> &move)
{
  std::for_each(move.begin(), move.end(), [this](const Decision &m) {
    _pattern_origin = Coordinates();
    ++_move_number;
    if (_variant == 1 or _variant == 2) {
      _previous_black_level = _black_level;
      _previous_white_level = _white_level;
    } else if (_variant == 3) {
      _previous_black_pattern_number = _black_pattern_number;
      _previous_white_pattern_number = _white_pattern_number;
    }
    if (m.type() == DecisionType::PUT_SHIDO) {
      _board[m.to().line()][m.to().column()] =
          m.color() == Color::BLACK ? State::BLACK_SHIDO : State::WHITE_SHIDO;
      if (_color == Color::BLACK) {

        // rule variant 2
        _black_possible_shido = false;

        --_black_shido_number;
      } else {

        // rule variant 2
        _white_possible_shido = false;

        --_white_shido_number;
      }
      std::vector<Coordinates> result = check_patterns();

      if (not result.empty()) {
        if (result.size() == 1) {
          capture(result[0]);
          block(result[0]);
          if (_variant == 1 or _variant == 2) {
            if (_color == Color::BLACK) {

              // rule variant 2
              _black_possible_shido = true;
              _white_possible_shido = is_possible_to_put_piece(Color::WHITE);

              ++_black_level;
            } else {

              // rule variant 2
              _white_possible_shido = true;
              _black_possible_shido = is_possible_to_put_piece(Color::BLACK);

              ++_white_level;
            }
          }
          change_color();
          next_phase();
        } else {
          _phase = Phase::CHOICE_PATTERN;
        }
      } else {
        change_color();
        next_phase();
      }
      _pass = 0;
    } else if (m.type() == DecisionType::PUT_PIECE) {
      _board[m.to().line()][m.to().column()] =
          m.color() == Color::BLACK ? State::BLACK : State::WHITE;
      if (_color == Color::BLACK) {
        --_black_piece_number;
      } else {
        --_white_piece_number;
      }
      _last_coordinates = m.to();

      std::vector<Coordinates> result = check_patterns();

      if (not result.empty()) {
        if (result.size() == 1) {
          capture(result[0]);
          block(result[0]);
          if (_variant == 1 or _variant == 2) {
            if (_color == Color::BLACK) {

              // rule variant 2
              _black_possible_shido = true;
              _white_possible_shido = is_possible_to_put_piece(Color::WHITE);

              ++_black_level;
            } else {

              // rule variant 2
              _white_possible_shido = true;
              _black_possible_shido = is_possible_to_put_piece(Color::BLACK);

              ++_white_level;
            }
          }
          change_color();
          next_phase();
        } else {
          _phase = Phase::CHOICE_PATTERN;
        }
      } else {
        change_color();
        next_phase();
      }
      _pass = 0;
    } else if (m.type() == DecisionType::CHOICE_PIECE) {
      if (m.index() == 0) {
        _phase = Phase::PUT_SHIDO;
      } else {
        _phase = Phase::PUT_PIECE;
      }
    } else if (m.type() == DecisionType::CHOICE_PATTERN) {
      std::vector<Coordinates> result = check_patterns();

      capture(result[m.index()]);
      block(result[m.index()]);
      if (_variant == 1 or _variant == 2) {
        if (_color == Color::BLACK) {
          _previous_black_level = _black_level;

          // rule variant 2
          _black_possible_shido = true;
          _white_possible_shido = is_possible_to_put_piece(Color::WHITE);

          ++_black_level;
        } else {
          _previous_white_level = _white_level;

          // rule variant 2
          _white_possible_shido = true;
          _black_possible_shido = is_possible_to_put_piece(Color::BLACK);

          ++_white_level;
        }
      }
      change_color();
      next_phase();
    } else if (m.type() == DecisionType::PASS) {
      ++_pass;
      _last_coordinates = Coordinates();
      change_color();
      next_phase();
    }

    // rule variant 2
    if (m.type() == DecisionType::PUT_INITIAL_SHIDO) {
      _board[m.to().line()][m.to().column()] =
          m.color() == Color::BLACK ? State::BLACK_SHIDO : State::WHITE_SHIDO;
      if (_color == Color::BLACK) {
        _black_possible_shido = false;
        --_black_shido_number;
      } else {
        _white_possible_shido = false;
        --_white_shido_number;
      }
      change_color();
      next_phase();
    }

  });
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
      std::to_string(_black_captured_shido_number) + " " +
      std::to_string(_white_captured_shido_number) + " " +
      std::to_string(_black_piece_number) + " " + std::to_string(_white_piece_number) + " " +
      std::to_string(_black_shido_number) + " " + std::to_string(_white_shido_number) + " " +
      std::to_string(_black_failed) + " " + std::to_string(_white_failed) + " "
      + std::to_string(_pass);

  // rule variant 2
  str += " " + std::to_string(_black_possible_shido) + " " + std::to_string(_white_possible_shido);

  // rule variant 3
  // TODO

  return str;
}

int Engine::winner_is() const
{
  if (is_finished()) {
    if (gain(Color::BLACK, true) > gain(Color::WHITE, true)) {
      return Color::BLACK;
    } else if (gain(Color::BLACK, true) < gain(Color::WHITE, true)) {
      return Color::WHITE;
    } else {
      return Color::NONE;
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

  _pattern_origin = origin;

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
  } else if (old_value == State::BLACK_SHIDO and _color == Color::BLACK) {
    new_value = State::BLACK_SHIDO_BLOCKED;
  } else if (old_value == State::WHITE_SHIDO and _color == Color::WHITE) {
    new_value = State::WHITE_SHIDO_BLOCKED;
  }
  _board[coordinates.line()][coordinates.column()] = new_value;
}

void Engine::capture(const Coordinates &origin)
{
  int l = origin.line();
  int c = origin.column();
  int n_piece = 0;
  int n_shido = 0;

  // rule variant 3
  int n_level = 0;

  while (l < origin.line() + 3) {
    if ((_board[l][c] == State::BLACK or _board[l][c] == State::BLACK_SHIDO)
        and _color == Color::WHITE) {
      _board[l][c] = State::VACANT;
      if (_board[l][c] == State::BLACK) {
        ++n_piece;
      } else {
        ++n_shido;
      }
    }
    if (_variant == 3) {
      if ((_board[l][c] == State::WHITE or _board[l][c] == State::WHITE_SHIDO)
          and _color == Color::WHITE) {
        ++n_level;
      }
    }
    if ((_board[l][c] == State::WHITE or _board[l][c] == State::WHITE_SHIDO)
        and _color == Color::BLACK) {
      _board[l][c] = State::VACANT;
      if (_board[l][c] == State::WHITE) {
        ++n_piece;
      } else {
        ++n_shido;
      }
    }
    if (_variant == 3) {
      if ((_board[l][c] == State::BLACK or _board[l][c] == State::BLACK_SHIDO)
          and _color == Color::BLACK) {
        ++n_level;
      }
    }
    ++c;
    if (c == origin.column() + 3) {
      c = origin.column();
      ++l;
    }
  }
  if (_color == Color::BLACK) {
    _black_captured_piece_number += n_piece;
    _black_captured_shido_number += n_shido;
    _black_piece_number += n_piece;
    if (_variant == 3) {
      _black_patterns.push_back(n_level - 2);
      ++_black_pattern_number;
    }
  } else {
    _white_captured_piece_number += n_piece;
    _white_captured_shido_number += n_shido;
    _white_piece_number += n_piece;
    if (_variant == 3) {
      _white_patterns.push_back(n_level - 2);
      ++_white_pattern_number;
    }
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

std::vector<Coordinates> Engine::check_pattern(const Pattern &pattern) const
{
  int l = 0;
  int c = 0;
  std::vector<Coordinates> origins;

  while (l < _size - 2) {
    if (check_no_blocked(Coordinates(c, l))) {
      if (check_pattern_in_block(Coordinates(c, l), pattern)) {
        origins.emplace_back(Coordinates(c, l));
      }
    }
    ++c;
    if (c == _size - 2) {
      c = 0;
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
        _board[l][c] != (_color == Color::BLACK ? State::BLACK : State::WHITE) and
        _board[l][c] != (_color == Color::BLACK ? State::BLACK_SHIDO : State::WHITE_SHIDO)) or
        (value == 1 and
            (_board[l][c] == (_color == Color::BLACK ? State::BLACK : State::WHITE) or
                _board[l][c] == (_color == Color::BLACK ? State::BLACK_SHIDO
                                                        : State::WHITE_SHIDO)))) {
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

std::vector<Coordinates> Engine::check_patterns() const
{
  if (_variant == 1 or _variant == 2) {
    int level = 0;
    bool found = false;
    std::vector<Coordinates> origins;

    while (!found and level < 5) {
      const LevelPattern &pattern = PATTERNS[level];

      for (const Pattern &p: pattern) {
        const std::vector<Coordinates> &new_origins = check_pattern(p);

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
        // TODO
//                            if (_color == Color::BLACK) {
//                                _black_failed = true;
//                            } else {
//                                _white_failed = true;
//                            }
        return std::vector<Coordinates>();
      }
    } else {
      return std::vector<Coordinates>();
    }
  } else if (_variant == 3) {
    bool found = false;
    std::vector<Coordinates> origins;

    for (int level = 0; level < 5; ++level) {
      const LevelPattern &pattern = PATTERNS[level];

      for (const Pattern &p: pattern) {
        const std::vector<Coordinates> &new_origins = check_pattern(p);

        if (not new_origins.empty()) {
          origins.insert(
              origins.end(),
              std::make_move_iterator(new_origins.begin()),
              std::make_move_iterator(new_origins.end())
          );
          found = true;
        }
      }
    }
    if (found) {
      return origins;
    } else {
      return std::vector<Coordinates>();
    }
  }
}

int Engine::count_possible_cases(const std::vector<std::vector<Possible_pattern_results>> &list) const
{
  int current_level = _color == Color::BLACK ? _black_level : _white_level;
  int counter = 0;

  for (LevelPattern::size_type i = 0; i < PATTERNS[current_level].size(); ++i) {
    counter += list[current_level][i].size();
  }
  return counter;
}

std::vector<Engine::Possible_pattern_results>
Engine::get_one_piece_pattern(const std::vector<std::vector<Possible_pattern_results>> &list) const
{
  std::vector<Possible_pattern_results> new_list;

  for (int level = 0; level < 5; ++level) {
    new_list.emplace_back(Possible_pattern_results());
    for (LevelPattern::size_type i = 0; i < PATTERNS[level].size(); ++i) {
      for (Possible_pattern_results::size_type index = 0;
           index < list[level][i].size(); ++index) {
        if (list[level][i][index].list.size() == 1) {
          new_list[level].push_back(list[level][i][index]);
        }
      }
    }
  }
  return new_list;
}

bool Engine::is_connect(const Coordinates &coordinates) const
{
  State::Values color = _color == Color::BLACK ? State::BLACK : State::WHITE;
  State::Values
      blocked_color = _color == Color::BLACK ? State::BLACK_BLOCKED : State::WHITE_BLOCKED;
  State::Values blocked_in_color =
      _color == Color::BLACK ? State::BLOCKED_IN_BLACK : State::BLOCKED_IN_WHITE;
  State::Values shido_color = _color == Color::BLACK ? State::BLACK_SHIDO : State::WHITE_SHIDO;
  State::Values shido_blocked_color =
      _color == Color::BLACK ? State::BLACK_SHIDO_BLOCKED : State::WHITE_SHIDO_BLOCKED;
  std::vector<Coordinates> list;
  std::vector<Coordinates> new_list;
  bool found = false;

  new_list.emplace_back(coordinates);
  while (not found and not new_list.empty()) {
    Coordinates current = new_list.back();

    new_list.pop_back();
    list.push_back(current);
    for (int i = -1; i < 2 and not found; ++i) {
      for (int j = -1; j < 2 and not found; ++j) {
        if (i != 0 or j != 0) {
          Coordinates new_element(current.column() + j, current.line() + i);

          if (new_element.line() >= 0 and new_element.line() < _size
              and new_element.column() >= 0
              and new_element.column() < _size) {
            if (std::find(list.begin(), list.end(), new_element) == list.end()) {
              State::Values cell_color = _board[new_element.line()][new_element.column()];

              if (cell_color == color or cell_color == blocked_color) {
                new_list.push_back(new_element);
              } else if (cell_color == shido_color or cell_color == shido_blocked_color
                  or cell_color == blocked_in_color) {
                found = true;
              }
            }
          }
        }
      }
    }
  }
  return found;
}

Engine::Possible_pattern_results Engine::is_possible_pattern(const Pattern &pattern) const
{
  Engine::Possible_pattern_results result;
  int l = 0;
  int c = 0;

  while (l < _size - 2) {
    Possible_pattern_result result_in_block = is_possible_pattern_in_block(Coordinates(c, l),
                                                                           pattern);

    if (result_in_block.ok) {
      result.push_back(result_in_block);
    }
    ++c;
    if (c == _size - 2) {
      c = 0;
      ++l;
    }
  }
  return result;
}

Engine::Possible_pattern_result
Engine::is_possible_pattern_in_block(const Coordinates &origin, const Pattern &pattern) const
{
  Engine::Possible_pattern_result result = {true, std::vector<Coordinates>()};
  int l = origin.line();
  int c = origin.column();
  State::Values color_state = _color == Color::BLACK ? State::BLACK : State::WHITE;
  State::Values
      shido_color_state = _color == Color::BLACK ? State::BLACK_SHIDO : State::WHITE_SHIDO;

  while (result.ok and l < origin.line() + 3) {
    bool value = pattern[l - origin.line()][c - origin.column()];
    State::Values state = _board[l][c];

    if (state != State::BLOCKED_IN_BLACK and state != State::BLOCKED_IN_WHITE
        and state != State::BLACK_BLOCKED and state != State::WHITE_BLOCKED and
        ((value == 0 and state != color_state and state != shido_color_state) or
            (value == 1 and (state == color_state or state == shido_color_state
                or state == State::VACANT)))) {
      if (value == 1 and state == State::VACANT) {
        result.list.emplace_back(Coordinates(c, l));
      }
      ++c;
      if (c == origin.column() + 3) {
        c = origin.column();
        ++l;
      }
    } else {
      result.ok = false;
    }
  }
  return result;
}

std::vector<std::vector<Engine::Possible_pattern_results>> Engine::is_possible_patterns() const
{
  std::vector<std::vector<Engine::Possible_pattern_results>> result;

  for (int level = 0; level < 5; ++level) {
    const LevelPattern &pattern = PATTERNS[level];

    result.emplace_back(std::vector<Engine::Possible_pattern_results>());
    for (const Pattern &p: pattern) {
      Engine::Possible_pattern_results result_pattern = is_possible_pattern(p);

      result[level].push_back(result_pattern);
    }
  }
  return result;
}

// rule variant 2
bool Engine::is_possible_to_put_piece(const Color &color) const
{
  openxum::core::common::Moves<Decision> moves;

  get_possible_put_piece(moves, color);
  return not moves.empty();
}

bool Engine::is_valid2(const Coordinates &coordinates) const
{
  int opponent_level = _color == Color::WHITE ? _black_level : _white_level;
  int level_distance = opponent_level == 0 ? 0 : opponent_level == 1 or opponent_level == 2 ? 1 : 2;

  return not _last_coordinates.is_valid()
      or (_last_coordinates.is_valid() and distance(coordinates) > level_distance);
}

void Engine::next_phase()
{
  if (_variant == 1 or _variant == 3) {
    if (_color == Color::BLACK) {
      if (_black_shido_number == 0) {
        _phase = Phase::PUT_PIECE;
      } else if (_black_shido_number == CONFIGURATIONS[_type].shido_number) {
        _phase = Phase::PUT_SHIDO;
      } else {
        _phase = Phase::CHOICE_PIECE;
      }
    } else {
      if (_white_shido_number == 0) {
        _phase = Phase::PUT_PIECE;
      } else if (_white_shido_number == CONFIGURATIONS[_type].shido_number) {
        _phase = Phase::PUT_SHIDO;
      } else {
        _phase = Phase::CHOICE_PIECE;
      }
    }
  } else if (_variant == 2) {
    if (_color == Color::BLACK) {
      if (_black_shido_number == CONFIGURATIONS[_type].shido_number) {
        _phase = Phase::PUT_INITIAL_SHIDO;
      } else if (_black_possible_shido) {
        if (_black_shido_number > 0) {
          _phase = Phase::CHOICE_PIECE;
        } else {
          _phase = Phase::PUT_PIECE;
        }
      } else {
        _phase = Phase::PUT_PIECE;
      }
    } else {
      if (_white_shido_number == CONFIGURATIONS[_type].shido_number) {
        _phase = Phase::PUT_INITIAL_SHIDO;
      } else if (_white_possible_shido) {
        if (_white_shido_number > 0) {
          _phase = Phase::CHOICE_PIECE;
        } else {
          _phase = Phase::PUT_PIECE;
        }
      } else {
        _phase = Phase::PUT_PIECE;
      }
    }
  }
}

bool Engine::possible_forbidden_pattern(const Coordinates &coordinates,
                                        const std::vector<Possible_pattern_results> &list) const
{
  int current_level = _color == Color::BLACK ? _black_level : _white_level;
  bool ok = true;
  int level = 0;

  while (ok and level < 5) {
    if (level != current_level) {
      Possible_pattern_results::size_type index = 0;

      while (ok and index < list[level].size()) {
        if (list[level][index].list[0].column() == coordinates.column() and
            list[level][index].list[0].line() == coordinates.line()) {
          ok = false;
        } else {
          ++index;
        }
      }
    }
    if (ok) {
      ++level;
    }
  }
  return not ok;
}

}
}
}
}