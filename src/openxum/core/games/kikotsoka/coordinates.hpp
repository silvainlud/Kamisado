/**
 * @file openxum/core/games/kikotsoka/coordinates.hpp
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

#ifndef OPENXUM_CORE_GAMES_KIKOTSOKA_COORDINATES_HPP
#define OPENXUM_CORE_GAMES_KIKOTSOKA_COORDINATES_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace openxum {
    namespace core {
        namespace games {
            namespace kikotsoka {

                class Coordinates {
                public:
                    explicit Coordinates(int = -1, int = -1);

                    int column() const { return _column; }

                    void from_object(const nlohmann::json& json)
                    {
                        _column = json["column"].get<int>();
                        _line = json["line"].get<int>();
                    }

                    bool is_valid() const { return _column != -1 and _line != -1; }

                    int line() const { return _line; }

                    bool operator==(const Coordinates& coordinates) const
                    {
                        return _line == coordinates.line() and _column == coordinates.column();
                    }

                    nlohmann::json to_object() const
                    {
                        nlohmann::json json;

                        json["column"] = _column;
                        json["line"] = _line;
                        return json;
                    }

                    std::string to_string() const { return char('A' + _column) + std::to_string(_line + 1); }

                private:
                    int _column;
                    int _line;
                };

            }
        }
    }
}

#endif