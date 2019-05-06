/**
 * @file openxum/ai/common/mcts_player.cpp
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

#include <random>

#include <openxum/ai/common/mcts_player.hpp>

namespace openxum {
    namespace ai {
        namespace common {

            // public methods
            openxum::core::common::Move* MCTSPlayer::get_move()
            {
                openxum::core::common::Move* move = nullptr;

                init_search();
                for (int i = 0; i < _simulation_number; ++i) {
                    simulate_one_game_from_root();
                }
                move = get_final_choice()->clone();
                delete _root;
                _root = nullptr;
                return move;
            }

            // private methods
            int MCTSPlayer::evaluate(const openxum::core::common::Engine* engine)
            {
                openxum::core::common::Engine* clone = engine->clone();
                int winner;

                while (not clone->is_finished()) {
                    play_a_random_turn(clone);
                }
                winner = clone->winner_is();
                delete clone;
                return winner;
            }

            void MCTSPlayer::init_search()
            {
                _root = new Node(engine().clone(), nullptr, nullptr);
            }

            void MCTSPlayer::simulate_one_game_from_root()
            {
                Node* current = _root;
                Node* node = current;
                openxum::core::common::Engine* monteCarloEngine = nullptr;

                //descent
                while (current != nullptr and not current->engine()->is_finished()) {
                    const std::vector<openxum::core::common::Move*>& possible_moves = current->get_possible_moves();

                    if (not possible_moves.empty()) {
                        node = current;
                        break;
                    } else {
                        node = current;
                        current = current->choice(current->engine()->current_color() == color());
                    }
                }

                if (not node->get_possible_moves().empty()) {
                    //expansion
                    if (current == nullptr or not current->engine()->is_finished()) {
                        current = node;

                        openxum::core::common::Engine* new_engine = current->engine()->clone();
                        openxum::core::common::Move* move = current->get_first_possible_move();
                        Node* newNode = new Node(new_engine, current, move->clone());

                        current->remove_first_possible_move();
                        current->add_children(newNode);
                        monteCarloEngine = newNode->engine();
                        current = newNode;
                    } else {
                        monteCarloEngine = current->engine();
                    }
                    updateScore(current, evaluate(monteCarloEngine));
                }
            }

            void MCTSPlayer::updateScore(Node* current, int winner)
            {
                while (current != nullptr) {
                    current->visit();
                    if (winner == color()) {
                        current->inc_wins();
                    } else {
                        current->inc_losses();
                    }
                    current = current->get_father();
                }
            }

            openxum::core::common::Move* MCTSPlayer::get_final_choice()
            {
                openxum::core::common::Move* finalChoice = nullptr;
                double best = -1;

                for (Node* child: _root->get_children()) {
                    double score = double(child->get_number_of_wins()) / child->get_visit_number();

                    if (score > best) {
                        best = score;
                        finalChoice = child->get_move();
                    }
                }
                return finalChoice;
            }

            void MCTSPlayer::play_a_random_turn(openxum::core::common::Engine* engine)
            {
                const openxum::core::common::Moves& list = engine->get_possible_move_list();

                if (not list.empty()) {
                    std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);
                    unsigned long index = distribution(_rng);
                    openxum::core::common::Move* move = list[index];

                    engine->move(move);
                }
            }

        }
    }
}