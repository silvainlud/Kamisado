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

                _move_number = 0;
                _new_move_number = 0;
                _evaluation_number = 0;
                init_search();
                for (unsigned int i = 0; i < _simulation_number; ++i) {
                    simulate_one_game_from_root();
                }
                move = get_final_choice()->clone();
                clear();
                return move;
            }

            // private methods
            void MCTSPlayer::add_children(Node* current)
            {
                if (not current->is_finished()) {
                    const openxum::core::common::Moves& possible_moves = current->get_possible_moves();

//                    _nodes[current->id()] = current;
                    for (auto move: possible_moves) {
                        Node* newNode = new Node(current->engine()->clone(), current, move->clone());
                        std::string id = newNode->id();

                        if (_nodes.find(id) == _nodes.end()) {
                            _nodes[id] = newNode;
                        } else {
                            delete newNode;
                            newNode = _nodes.find(id)->second;
                        }
                        current->add_children(newNode);
                    }
                }
            }

            void MCTSPlayer::clear()
            {
                _root = nullptr;
                for (auto const& n: _nodes) {
                    delete n.second;
                }
                _nodes.clear();
            }

            Node* MCTSPlayer::descent() const
            {
                Node* current = _root;

                while (current != nullptr and not current->is_finished()
                        and current->get_unvisited_child_number() == 0) {
                    current = current->choice();
                }
                return current;
            }

            int MCTSPlayer::evaluate(const openxum::core::common::Engine* engine, openxum::core::common::Engine* clone)
            {
//                std::vector<std::string> ids;
                int winner = -1;

                ++_evaluation_number;
                if (_stoppable) {
                    while (not clone->is_stoppable()) {
                        play_a_turn_randomly(clone);
//                        ids.push_back(clone->id());
                    }
                    winner = clone->best_is();
                } else {
                    while (not clone->is_finished()) {
                        play_a_turn_randomly(clone);
//                        ids.push_back(clone->id());
                    }
                    winner = clone->winner_is();
                }
                _move_number += clone->move_number();
                _new_move_number += clone->move_number() - engine->move_number();
//                for (const std::string& id: ids) {
//                    if (_states.find(id) == _states.end()) {
//                        _states[id] = {winner == color() ? 1 : 0, winner == opponent_color() ? 1 : 0, 1};
//                    } else {
//                        _states[id]._win_number += winner == color() ? 1 : 0;
//                        _states[id]._loss_number += winner == opponent_color() ? 1 : 0;
//                        _states[id]._visit_number++;
//                    }
//                }
                return winner;
            }

            openxum::core::common::Move* MCTSPlayer::get_final_choice()
            {
                openxum::core::common::Move* finalChoice = nullptr;
                double best = -1;
                int best_visit = 0;

                for (Node* child: _root->get_children()) {
                    double score = child->get_visit_number() == 0 ? 0 : child->get_number_of_wins()
                            / child->get_visit_number();

                    if (score > best) {
                        best = score;
                        best_visit = child->get_visit_number();
                        finalChoice = child->get_move();
                    }
                }

                // **** TRACE
//                std::cout << _root->get_children().size() << ";" << _root->get_possible_move_number() << ";"
//                          << _root->max_depth() << ";" << (double(_move_number) / _evaluation_number) << ";"
//                          << (double(_new_move_number) / _evaluation_number) << ";" << _evaluation_number << std::endl;
//                for (Node* child: _root->get_children()) {
//                    double score = child->get_visit_number() == 0 ? 0 : child->get_number_of_wins()
//                            / child->get_visit_number();
//
//                    std::cout << child->get_move()->to_string() << " => " << score << " " << child->get_number_of_wins()
//                              << " " << child->get_visit_number() << std::endl;
//                }
//                std::cout << best << ";" << best_visit << ";" << finalChoice->to_string() << std::endl;
                // **** TRACE
                return finalChoice;
            }

            void MCTSPlayer::init_search()
            {
                _root = new Node(engine().clone(), nullptr, nullptr);
                add_children(_root);
            }

            void MCTSPlayer::play_a_turn_randomly(openxum::core::common::Engine* engine)
            {
                const openxum::core::common::Moves& list = engine->get_possible_move_list();

                if (not list.empty()) {
                    std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);
                    unsigned long index = distribution(_rng);
                    openxum::core::common::Move* move = list[index];

                    engine->move(move);
                }
            }

            void MCTSPlayer::simulate_one_game_from_root()
            {
                Node* current = descent();

                if (current != nullptr and current->get_unvisited_child_number() > 0) {
                    openxum::core::common::Engine* currentEngine;

                    if (not current->engine()->is_finished()) {
                        openxum::ai::common::Node* child = current->get_next_unvisited_child();

                        add_children(child);
                        currentEngine = child->engine();
                        current = child;
                    } else {
                        currentEngine = current->engine();
                    }

                    openxum::core::common::Engine* engine = currentEngine->clone();

                    updateScore(current, evaluate(currentEngine, engine), *engine);
                    delete engine;
                }
            }

            void MCTSPlayer::updateScore(Node* current, int winner, const openxum::core::common::Engine& engine)
            {
                while (current != nullptr) {
                    current->visit();
                    if (winner == color()) {
                        current->inc_wins(engine.gain());
                    } else {
                        current->inc_losses(engine.gain());
                    }
                    current = current->get_father();
                }
            }

        }
    }
}