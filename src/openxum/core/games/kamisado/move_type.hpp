/**
 * @file openxum/core/games/kamisado/move_type.hpp
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

#ifndef OPENXUM_CORE_GAMES_KAMISADO_MOVE_TYPE_HPP
#define OPENXUM_CORE_GAMES_KAMISADO_MOVE_TYPE_HPP

#include <openxum/core/common/move.hpp>
#include <openxum/core/games/kamisado/coordinates.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kamisado {

enum MoveType
{
  MOVE = 0, PASS = 1
};

}
}
}
}

#endif