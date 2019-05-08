/**
 * @file openxum/core/games/kamisado/coordinates.hpp
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

#ifndef OPENXUM_CORE_GAMES_KAMISADO_COORDINATES_HPP
#define OPENXUM_CORE_GAMES_KAMISADO_COORDINATES_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace openxum {
    namespace core {
        namespace games {
            namespace kamisado {

                class Coordinates {
                public:
                    explicit Coordinates(int = -1, int = -1);

                    void from_object(const nlohmann::json& json)
                    {
                        _x = json["x"].get<int>();
                        _y = json["y"].get<int>();
                    }

                    bool is_valid() const { return _x != -1 and _y != -1; }

                    nlohmann::json to_object() const
                    {
                        nlohmann::json json;

                        json["x"] = _x;
                        json["y"] = _y;
                        return json;
                    }

                    std::string to_string() const { return char('a' + _x) + std::to_string(_y + 1); }

                    int x() const { return _x; }

                    int y() const { return _y; }

                private:
                    int _x;
                    int _y;
                };

            }
        }
    }
}

#endif