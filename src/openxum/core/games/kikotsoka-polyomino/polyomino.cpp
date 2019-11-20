/**
 * @file openxum/core/games/kikotsoka-polyomino/polyomino.cpp
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

#include <openxum/core/games/kikotsoka-polyomino/polyomino.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kikotsoka_polyomino {

Polyomino::Polyominos Polyomino::POLYOMINOS = {
    {
        {{false, true, false}, {false, true, false}, {false, true, false}},
        {{false, false, false}, {true, true, true}, {false, false, false}}
    },
    {
        {{false, true, false}, {true, true, true}, {false, true, false}}
    },
    {
        {{false, true, false}, {false, true, false}, {true, true, false}},
        {{true, false, false}, {true, true, true}, {false, false, false}},
        {{false, true, true}, {false, true, false}, {false, true, false}},
        {{false, false, false}, {true, true, true}, {false, false, true}}
    },
    {
        {{false, true, false}, {false, true, false}, {false, true, true}},
        {{false, false, false}, {true, true, true}, {true, false, false}},
        {{true, true, false}, {false, true, false}, {false, true, false}},
        {{false, false, true}, {true, true, true}, {false, false, false}}
    },
    {
        {{false, false, false}, {true, true, true}, {false, true, false}},
        {{false, true, false}, {true, true, false}, {false, true, false}},
        {{false, true, false}, {true, true, true}, {false, false, false}},
        {{false, true, false}, {false, true, true}, {false, true, false}}
    },
    {
        {{true, true, true}, {false, true, false}, {false, true, false}},
        {{false, false, true}, {true, true, true}, {false, false, true}},
        {{false, true, false}, {false, true, false}, {true, true, true}},
        {{true, false, false}, {true, true, true}, {true, false, false}}
    },
    {
        {{false, true, true}, {false, true, false}, {true, true, false}},
        {{true, false, false}, {true, true, true}, {false, false, true}}
    },
    {
        {{true, true, false}, {false, true, false}, {false, true, true}},
        {{false, false, true}, {true, true, true}, {true, false, false}}
    },
    {
        {{false, false, false}, {true, false, true}, {true, true, true}},
        {{true, true, false}, {true, false, false}, {true, true, false}},
        {{true, true, true}, {true, false, true}, {false, false, false}},
        {{false, true, true}, {false, false, true}, {false, true, true}}
    },
    {
        {{false, true, false}, {true, true, false}, {true, true, false}},
        {{true, true, false}, {true, true, true}, {false, false, false}},
        {{false, true, true}, {false, true, true}, {false, true, false}},
        {{false, false, false}, {true, true, true}, {false, true, true}}
    },
    {
        {{false, true, false}, {false, true, true}, {false, true, true}},
        {{false, false, false}, {true, true, true}, {true, true, false}},
        {{true, true, false}, {true, true, false}, {false, true, false}},
        {{false, true, true}, {true, true, true}, {false, false, false}}
    },
    {
        {{true, false, false}, {true, false, false}, {true, true, true}},
        {{true, true, true}, {true, false, false}, {true, false, false}},
        {{true, true, true}, {false, false, true}, {false, false, true}},
        {{false, false, true}, {false, false, true}, {true, true, true}}
    },
    {
        {{false, true, false}, {false, true, true}, {true, true, false}},
        {{true, false, false}, {true, true, true}, {false, true, false}},
        {{false, true, true}, {true, true, false}, {false, true, false}},
        {{false, true, false}, {true, true, true}, {false, false, true}}
    },
    {
        {{false, true, false}, {true, true, false}, {false, true, true}},
        {{false, true, false}, {true, true, true}, {true, false, false}},
        {{true, true, false}, {false, true, true}, {false, true, false}},
        {{false, false, true}, {true, true, true}, {false, true, false}}
    }
};

Polyomino::Polyomino(int shape_index, const Matrix_3_3 &points)
    : _shape_index(shape_index), _rotation(0)
{
  const Shape &p = POLYOMINOS[_shape_index][0];

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      _points[i][j] = points[i][j];
      _shape[i][j] = p[i][j];
      if (_shape[i][j] == 1) {
        _shape[i][j] += _points[i][j];
      }
    }
  }
}

Polyomino *Polyomino::clone() const
{
  auto o = new Polyomino(_shape_index, _points);

  o->_rotation = _rotation;
  o->_coordinates = _coordinates;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      o->_shape[i][j] = _shape[i][j];
    }
  }
  return o;
}

void Polyomino::rotate()
{
  ++_rotation;
  _rotation = _rotation % 4;
  for (int i = 0; i < 1; i++) {
    for (int j = i; j < 3 - i - 1; j++) {
      int temp = _shape[i][j];

      _shape[i][j] = _shape[3 - 1 - j][i];
      _shape[3 - 1 - j][i] = _shape[3 - 1 - i][3 - 1 - j];
      _shape[3 - 1 - i][3 - 1 - j] = _shape[j][3 - 1 - i];
      _shape[j][3 - 1 - i] = temp;
    }
  }
}

std::string Polyomino::to_string() const
{
  std::string str;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      str += _shape[i][j] == 0 ? " " : _shape[i][j] == 1 ? "o" : "O";
    }
    str += "\n";
  }
  return str;
}

}
}
}
}