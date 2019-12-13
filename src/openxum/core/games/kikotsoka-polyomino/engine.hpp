/**
 * @file openxum/core/games/kikotsoka-polyomino/engine.hpp
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

#ifndef OPENXUM_CORE_GAMES_KIKOTSOKA_POLYOMINO_ENGINE_HPP
#define OPENXUM_CORE_GAMES_KIKOTSOKA_POLYOMINO_ENGINE_HPP

#include <openxum/core/common/engine.hpp>

#include <openxum/core/games/kikotsoka-polyomino/coordinates.hpp>
#include <openxum/core/games/kikotsoka-polyomino/decision.hpp>
#include <openxum/core/games/kikotsoka-polyomino/phase.hpp>
#include <openxum/core/games/kikotsoka-polyomino/polyomino.hpp>
#include <openxum/core/games/kikotsoka-polyomino/state.hpp>

#include <vector>

namespace openxum {
namespace core {
namespace games {
namespace kikotsoka_polyomino {

class Engine : public openxum::core::common::Engine<Decision>
{
public:
  Engine() = default;

  Engine(int type, int color);

  ~Engine() override;

  int best_is() const override;

  Engine *clone() const override;

  int current_color() const override
  { return _color; }

  double gain(int color) const override;

  const std::string &get_name() const override
  { return GAME_NAME; }

  openxum::core::common::Moves<Decision> get_possible_move_list() const override;

  std::string id() const override;

  bool is_finished() const override;

  bool is_stoppable() const override;

  void move(const openxum::core::common::Move<Decision>& move) override;

  void parse(const std::string &) override
  {}

  int type() const
  { return _type; }

  std::string to_string() const override;

  int winner_is() const override;

  struct Configuration
  {
    int size;
    int polyomino_number;
  };

  static Configuration CONFIGURATIONS[1];

  int _black_level;
  int _white_level;

private:
  typedef std::vector<bool> PatternLine;
  typedef std::vector<PatternLine> Pattern;
  typedef std::vector<Pattern> LevelPattern;
  typedef std::vector<LevelPattern> Patterns;

  class Failed : public std::exception
  {
  };

  void block(const Coordinates &origin);

  void block_coordinates(const Coordinates &coordinates);

  void capture(const Coordinates &origin);

  void change_color()
  {
    _color = _color == Color::BLACK ? Color::WHITE : Color::BLACK;
  }

  bool check_no_blocked(const Coordinates &origin) const;

  std::vector<Coordinates> check_pattern(const Pattern &pattern,
                                         const Coordinates &coordinates) const;

  bool check_pattern_in_block(const Coordinates &origin, const Pattern &pattern) const;

  std::vector<Coordinates> check_patterns(const Coordinates &coordinates) const;

  void init_polyominos(const Color &color, std::vector<Polyomino *> &polyominos, int size);

  bool is_neighbour(int i, int j) const;

  bool is_valid(Polyomino *polyomino, const Coordinates &coordinates, bool first);

  void put_polyomino(Polyomino *polyomino, const Coordinates &coordinates);

  void remove_polyomino(Polyomino *polyomino, const Coordinates &coordinates);

  Color _color;
  int _type;
  Phase::Values _phase;
  State::Values **_board;
  std::vector<Polyomino *> _black_polyominos;
  std::vector<Polyomino *> _white_polyominos;
  int _black_polyomino_number;
  int _white_polyomino_number;
  Coordinates _last_coordinates;
  int _pass;
  int _black_captured_piece_number;
  int _white_captured_piece_number;
  int _previous_black_level;
  int _previous_white_level;
  bool _black_failed;
  bool _white_failed;
  int _size;

  static Patterns PATTERNS;
  static std::string GAME_NAME;
  static std::vector<std::vector<std::vector<int> > > BLACK_POINTS;
  static std::vector<std::vector<std::vector<int> > > WHITE_POINTS;
  static std::vector<int> BLACK_INDEXES;
  static std::vector<int> WHITE_INDEXES;
};

}
}
}
}

#endif