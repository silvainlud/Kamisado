/**
 * @file openxum/core/common/mcts_player.hpp
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

#ifndef OPENXUM_AI_SPECIFIC_KAMISADO_MCTS_PLAYER_HPP
#define OPENXUM_AI_SPECIFIC_KAMISADO_MCTS_PLAYER_HPP

#include <openxum/ai/common/mcts_player.hpp>

namespace openxum {
    namespace ai {
        namespace specific {
            namespace kamisado {

                class MCTSPlayer : public openxum::ai::common::MCTSPlayer {
                public:
                    MCTSPlayer(int c, int o, openxum::core::common::Engine* e)
                            :openxum::ai::common::MCTSPlayer(c, o, e, 10000) { }
                };

            }
        }
    }
}

#endif