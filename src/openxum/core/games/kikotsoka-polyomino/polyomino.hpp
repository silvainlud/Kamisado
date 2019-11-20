/**
 * @file openxum/core/games/kikotsoka-polyomino/polyomino.hpp
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

#ifndef OPENXUM_CORE_GAMES_KIKOTSOKA_POLYOMINO_POLYOMINO_HPP
#define OPENXUM_CORE_GAMES_KIKOTSOKA_POLYOMINO_POLYOMINO_HPP

#include <openxum/core/games/kikotsoka-polyomino/coordinates.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kikotsoka_polyomino {

class Polyomino
{
public:
  typedef int Matrix_3_3[3][3];
  typedef std::vector<bool> ShapeLine;
  typedef std::vector<ShapeLine> Shape;
  typedef std::vector<Shape> Shapes;
  typedef std::vector<Shapes> Polyominos;

  static Polyominos POLYOMINOS;

  Polyomino(int shape_index, const Matrix_3_3 &points);

  Polyomino *clone() const;

  const Coordinates &coordinates() const
  { return _coordinates; }

  bool is_free() const
  { return not _coordinates.is_valid(); }

  void put(const Coordinates &coordinates)
  { _coordinates = coordinates; }

  void remove()
  { _coordinates = Coordinates(); }

  void rotate();

  int rotation() const
  { return _rotation; }

  const Matrix_3_3 &shape() const
  { return _shape; }

  int shape_index() const
  { return _shape_index; }

  std::string to_string() const;

private:
  int _shape_index;
  int _rotation;
  Matrix_3_3 _points;
  Coordinates _coordinates;
  Matrix_3_3 _shape;
};

}
}
}
}

#endif