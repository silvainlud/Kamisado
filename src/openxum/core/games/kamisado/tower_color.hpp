/**
 * @file openxum/core/games/kamisado/tower_color.hpp
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

#ifndef OPENXUM_CORE_GAMES_KAMISADO_TOWER_COLOR_HPP
#define OPENXUM_CORE_GAMES_KAMISADO_TOWER_COLOR_HPP

namespace openxum {
    namespace core {
        namespace games {
            namespace kamisado {

                class TowerColor {
                public:
                    enum Values {
                        NONE = -1, ORANGE = 0, BLUE, PURPLE, PINK, YELLOW, RED, GREEN, BROWN
                    };

                    static std::string to_string(const TowerColor::Values& c)
                    {
                        if (c == NONE) { return "none"; }
                        if (c == ORANGE) { return "orange"; }
                        if (c == BLUE) { return "blue"; }
                        if (c == PURPLE) { return "purple"; }
                        if (c == PINK) { return "pink"; }
                        if (c == YELLOW) { return "yellow"; }
                        if (c == RED) { return "red"; }
                        if (c == GREEN) { return "green"; }
                        if (c == BROWN) { return "brown"; }
                        return "";
                    }
                };

            }
        }
    }
}

#endif