/**
 * @file openxum/core/games/kamisado/move.hpp
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

#ifndef OPENXUM_CORE_GAMES_KAMISADO_MOVE_HPP
#define OPENXUM_CORE_GAMES_KAMISADO_MOVE_HPP

#include <openxum/core/common/move.hpp>
#include <openxum/core/games/kamisado/coordinates.hpp>

namespace openxum {
    namespace core {
        namespace games {
            namespace kamisado {

                class Move : public openxum::core::common::Move {
                public:
                    Move() = default;

                    Move(const Coordinates& to, const Coordinates& from);

                    void decode(const std::string&) override;

                    std::string encode() const override;

                    const Coordinates& from() const { return _from; }

                    void from_object(const nlohmann::json&) override;

                    const Coordinates& to() const { return _to; }

                    nlohmann::json to_object() const override;

                    std::string to_string() const override
                    {
                        return "move tower from " + _from.to_string() + " to " + _to.to_string();
                    }

                private:
                    Coordinates _from;
                    Coordinates _to;
                };

            }
        }
    }
}

#endif