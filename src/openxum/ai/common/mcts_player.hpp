/**
 * @file openxum/ai/common/mcts_player.hpp
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

#ifndef OPENXUM_AI_COMMON_MCTS_PLAYER_HPP
#define OPENXUM_AI_COMMON_MCTS_PLAYER_HPP

#include <openxum/core/common/player.hpp>
#include <openxum/core/common/engine.hpp>
#include <openxum/ai/common/node.hpp>
#include <random>

namespace openxum {
    namespace ai {
        namespace common {

            class MCTSPlayer : public openxum::core::common::Player {
            public:
                MCTSPlayer(int c, int o, openxum::core::common::Engine* e, unsigned int simulation_number,
                        bool stoppable = false)
                        :openxum::core::common::Player(c, o, e), _simulation_number(simulation_number),
                         _stoppable(stoppable),
                         _root(nullptr), _rng(_random_device()) { }

                ~MCTSPlayer() override { delete _root; }

                openxum::core::common::Move* get_move() override;

            private:
                void add_children(Node* current);

                void clear();

                Node* descent() const;

                int evaluate(const openxum::core::common::Engine*);

                openxum::core::common::Move* get_final_choice();

                void init_search();

                void play_a_turn_randomly(openxum::core::common::Engine* engine);

                bool simulate_one_game_from_root();

                void updateScore(Node*, int);

                struct State {
                    int _win_number;
                    int _loss_number;
                    int _visit_number;
                };

                unsigned int _simulation_number;
                bool _stoppable;
                unsigned int _move_number;
                unsigned int _new_move_number;
                unsigned int _evaluation_number;
                Node* _root;
                std::random_device _random_device;
                std::mt19937 _rng;
                std::map<std::string, Node*> _nodes;
                std::map<std::string, State> _states;
            };

        }
    }
}

#endif