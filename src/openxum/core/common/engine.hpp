/**
 * @file openxum/core/common/engine.hpp
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

#ifndef OPENXUM_CORE_COMMON_ENGINE_HPP
#define OPENXUM_CORE_COMMON_ENGINE_HPP

#include <openxum/core/common/move.hpp>

namespace openxum {
    namespace core {
        namespace common {

            class Engine {
            public:
                Engine()
                {
                }

                virtual void apply_moves(const Moves& moves) = 0;

                virtual Engine* clone() const = 0;

                virtual int current_color() const = 0;

                virtual const std::string& get_name() const = 0;

                virtual Moves get_possible_move_list() const = 0;

                virtual bool is_finished() const = 0;

                virtual void move(const Move* move) = 0;

                virtual void parse(const std::string& str) = 0;

                virtual std::string to_string() const = 0;

                virtual int winner_is() const = 0;
            };

        }
    }
}

#endif