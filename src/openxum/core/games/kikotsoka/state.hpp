/**
 * @file openxum/core/games/kikotsoka/state.hpp
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

#ifndef OPENXUM_CORE_GAMES_KIKOTSOKA_STATE_HPP
#define OPENXUM_CORE_GAMES_KIKOTSOKA_STATE_HPP

#include <string>

namespace openxum {
    namespace core {
        namespace games {
            namespace kikotsoka {

                class State {
                public:
                    enum Values {
                        VACANT = 0,
                        BLACK = 1,
                        WHITE = 2,
                        BLACK_BLOCKED = 3,
                        WHITE_BLOCKED = 4,
                        BLACK_SHIDO = 5,
                        WHITE_SHIDO = 6,
                        BLACK_SHIDO_BLOCKED = 7,
                        WHITE_SHIDO_BLOCKED = 8,
                        BLOCKED_IN_BLACK = 9,
                        BLOCKED_IN_WHITE = 10
                    };

                    static std::string to_string(const State::Values& s)
                    {
                        if (s == VACANT) { return "[ ]"; }
                        if (s == BLACK) { return "[B]"; }
                        if (s == WHITE) { return "[W]"; }
                        if (s == BLACK_BLOCKED) { return " b "; }
                        if (s == WHITE_BLOCKED) { return " w "; }
                        if (s == BLACK_SHIDO) { return "*b*"; }
                        if (s == WHITE_SHIDO) { return "*w*"; }
                        if (s == BLACK_SHIDO_BLOCKED) { return "+b+"; }
                        if (s == WHITE_SHIDO_BLOCKED) { return "+w+"; }
                        if (s == BLOCKED_IN_BLACK || s == BLOCKED_IN_WHITE) { return ""   ""; }
                    }
                };

            }
        }
    }
}

#endif