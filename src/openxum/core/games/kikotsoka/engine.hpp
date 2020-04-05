/**
 * @file openxum/core/games/kikotsoka/engine.hpp
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

#ifndef OPENXUM_CORE_GAMES_KIKOTSOKA_ENGINE_HPP
#define OPENXUM_CORE_GAMES_KIKOTSOKA_ENGINE_HPP

#include <openxum/core/common/engine.hpp>

#include <openxum/core/games/kikotsoka/coordinates.hpp>
#include <openxum/core/games/kikotsoka/decision.hpp>
#include <openxum/core/games/kikotsoka/phase.hpp>
#include <openxum/core/games/kikotsoka/state.hpp>

#include <vector>

namespace openxum {
namespace core {
namespace games {
namespace kikotsoka {

class Engine : public openxum::core::common::Engine<kikotsoka::Decision>
{
public:
  Engine() = default;

  Engine(int type, int color, int variant);

  ~Engine() override;

  int best_is() const override;

//  openxum::core::common::Move<kikotsoka::Decision> *build_move() const override
//  { return new common::Move<kikotsoka::Decision>(); }

  Engine *clone() const override;

  int current_color() const override
  { return _color; }

  unsigned int current_goal(int color) const override
  { return color == Color::BLACK ? _black_level : _white_level; }

  double gain(int color, bool finish) const override;

  unsigned int get_goal_number() const override
  { return 5; }

  const std::string &get_name() const override
  { return GAME_NAME; }

  openxum::core::common::Moves<kikotsoka::Decision> get_possible_move_list() const override;

  std::string id() const override;

  bool is_finished() const override;

  bool is_stoppable() const override;

  void move(const openxum::core::common::Move<kikotsoka::Decision> &move) override;

  void parse(const std::string &) override
  {}

  int type() const
  { return _type; }

  std::string to_string() const override;

  int winner_is() const override;

  struct Configuration
  {
    int size;
    int piece_number;
    int shido_number;
  };

  static Configuration CONFIGURATIONS[3];

  // rule variant 1 and 2
  int _black_level;
  int _white_level;

  // rule variant 1, 2 and 3
  int _black_captured_piece_number;
  int _white_captured_piece_number;
  int _black_captured_shido_number;
  int _white_captured_shido_number;
  Coordinates _pattern_origin;

  // rule variant 3
  int _black_pattern_number;
  int _white_pattern_number;
  std::vector < int > _black_patterns;
  std::vector < int > _white_patterns;

private:
  typedef std::vector<bool> PatternLine;
  typedef std::vector<PatternLine> Pattern;
  typedef std::vector<Pattern> LevelPattern;
  typedef std::vector<LevelPattern> Patterns;

  struct Possible_pattern_result
  {
    bool ok;
    std::vector<Coordinates> list;
  };

  typedef std::vector<Possible_pattern_result> Possible_pattern_results;

  void block(const Coordinates &origin);

  void block_coordinates(const Coordinates &coordinates);

  void capture(const Coordinates &origin);

  void change_color()
  {
    _color = _color == Color::BLACK ? Color::WHITE : Color::BLACK;
  }

  bool check_no_blocked(const Coordinates &origin) const;

  std::vector<Coordinates> check_pattern(const Pattern &pattern) const;

  bool check_pattern_in_block(const Coordinates &origin, const Pattern &pattern) const;

  std::vector<Coordinates> check_patterns() const;

  // rule variants 1 and 2
  int count_possible_cases(const std::vector<std::vector<Possible_pattern_results>> &list) const;

  int distance(const Coordinates &coordinates) const
  {
    return std::abs(_last_coordinates.line() - coordinates.line()) +
        std::abs(_last_coordinates.column() - coordinates.column());
  }

  // rule variants 1 and 2
  std::vector<Possible_pattern_results>
  get_one_piece_pattern(const std::vector<std::vector<Possible_pattern_results>> &list) const;

  void get_possible_put_piece(common::Moves<kikotsoka::Decision> &moves,
                              const Color &color, bool decision = false) const;

  void get_possible_put_shido(common::Moves<kikotsoka::Decision> &moves,
                              bool decision = false) const;

  bool is_connect(const Coordinates &coordinates) const;

  bool is_possible_to_put_piece(const Color &color) const;

  Possible_pattern_results is_possible_pattern(const Pattern &pattern) const;

  Possible_pattern_result
  is_possible_pattern_in_block(const Coordinates &origin, const Pattern &pattern) const;

  std::vector<std::vector<Possible_pattern_results>> is_possible_patterns() const;

  bool is_valid(const Coordinates &coordinates) const
  {
    return is_valid2(coordinates)
        and _board[coordinates.line()][coordinates.column()] == State::VACANT;
  }

  bool is_valid2(const Coordinates &coordinates) const;

  void next_phase();

  // rule variants 1 and 2
  bool possible_forbidden_pattern(const Coordinates &coordinates,
                                  const std::vector<Possible_pattern_results> &list) const;

  Color _color;
  int _type;
  Phase::Values _phase;
  State::Values **_board;
  int _black_piece_number;
  int _white_piece_number;
  int _black_shido_number;
  int _white_shido_number;
  Coordinates _last_coordinates;
  int _pass;
  int _previous_black_level;
  int _previous_white_level;
  bool _black_failed;
  bool _white_failed;
  int _size;
  int _variant;

  // rule variant 2
  bool _black_possible_shido;
  bool _white_possible_shido;

  // rule variant 3
  int _previous_black_pattern_number;
  int _previous_white_pattern_number;

  static Patterns PATTERNS;
  static std::string GAME_NAME;
};

}
}
}
}

#endif