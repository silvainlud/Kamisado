/**
 * @file openxum/ai/common/random_player.hpp
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

#ifndef OPENXUM_AI_COMMON_RANDOM_PLAYER_HPP
#define OPENXUM_AI_COMMON_RANDOM_PLAYER_HPP

#include <random>
#include <openxum/core/common/player.hpp>
#include <openxum/core/common/engine.hpp>

namespace openxum {
    namespace ai {
        namespace common {

            class RandomPlayer : public openxum::core::common::Player {
            public:
                RandomPlayer(int c, int o, openxum::core::common::Engine* e)
                        :openxum::core::common::Player(c, o, e), _rng(_random_device()) { }

                openxum::core::common::Move* get_move() override;

            private:
                std::random_device _random_device;
                std::mt19937 _rng;
            };

        }
    }
}

#endif