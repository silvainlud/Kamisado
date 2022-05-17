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
#include <iostream>

namespace openxum {
    namespace ai {
        namespace common {

            template<class Decision>
            class MCTSPlayer : public openxum::core::common::Player<Decision> {
            public:
                MCTSPlayer(int c,
                           int o,
                           openxum::core::common::Engine<Decision> *e,
                           unsigned int simulation_number,
                           bool stoppable = false)
                        : openxum::core::common::Player<Decision>(c, o, e), _simulation_number(simulation_number),
                          _stoppable(stoppable),
                          _root(nullptr), _rng(_random_device()) {}

                ~MCTSPlayer() override { delete _root; }

                openxum::core::common::Move<Decision> get_move() override {
                    openxum::core::common::Move<Decision> move;

                    _move_number = 0;
                    _new_move_number = 0;
                    _evaluation_number = 0;
                    _best = nullptr;
                    init_search();
                    for (unsigned int i = 0; i < _simulation_number; ++i) {
                        simulate_one_game_from_root();
                    }
                    _best = get_final_choice();
                    move = _best->get_move();
                    compute_next_goal_distance(move);
                    _next_goal_distance_evaluation = _best->get_visit_number();
                    clear();
                    return move;
                }

                double get_next_goal_distance() override { return _next_goal_distance; }

                unsigned int get_next_goal_distance_evaluation() override { return _next_goal_distance_evaluation; }

            public:

                void set_simulation_number(uint simulation_number) {
                    _simulation_number = simulation_number;
                }

                uint get_simulation_number() {
                    return _simulation_number;
                }

                void add_children(Node<Decision> *current) {
                    if (not current->is_finished()) {
                        const openxum::core::common::Moves<Decision> &possible_moves = current->get_possible_moves();

                        for (auto move: possible_moves) {
                            Node<Decision>
                                    *newNode = new Node<Decision>(current->engine()->clone(), current, move->clone());
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

                void clear() {
                    delete _root;
                    _root = nullptr;
                    for (auto const &n: _nodes) {
                        delete n.second;
                    }
                    _nodes.clear();
                }

                void compute_next_goal_distance(
                        const openxum::core::common::Move<Decision> & /* move */) { _next_goal_distance = _best->get_next_goal_distance(); }

                Node<Decision> *descent() const {
                    Node<Decision> *current = _root;

                    while (current != nullptr and not current->is_finished()
                           and current->get_unvisited_child_number() == 0) {
                        current = current->choice();
                    }
                    return current;
                }

                void evaluate(const openxum::core::common::Engine<Decision> *engine,
                              openxum::core::common::Engine<Decision> *clone,
                              int &winner,
                              unsigned int &distance) {
                    winner = -1;
                    distance = 0;
                    ++_evaluation_number;
                    if (_stoppable) {
                        while (not clone->is_stoppable()) {
                            play_a_turn_randomly(clone);
                        }
                        winner = clone->best_is();
                    } else {
                        while (not clone->is_finished()) {
                            if (_root->engine()->current_goal(this->color()) == clone->current_goal(this->color())) {
                                ++distance;
                            }
                            play_a_turn_randomly(clone);
                        }
                        winner = clone->winner_is();
                    }
                    _move_number += clone->move_number();
                    _new_move_number += clone->move_number() - engine->move_number();
                }

                openxum::ai::common::Node<Decision> *get_final_choice() {
                    openxum::ai::common::Node<Decision> *finalChoice = nullptr;
                    double best = -1;

                    for (Node<Decision> *child: _root->get_children()) {
                        double score = child->get_visit_number() == 0 ? 0 : child->get_number_of_wins()
                                                                            / child->get_visit_number();

                        if (score > best) {
                            best = score;
                            finalChoice = child;
                        }
                    }
                    return finalChoice;
                }

                void init_search() {
                    _root = new Node<Decision>(this->engine().clone(), nullptr,
                                               openxum::core::common::Move<Decision>());
                }

                void play_a_turn_randomly(openxum::core::common::Engine<Decision> *engine) {
                    const openxum::core::common::Moves<Decision> &list = engine->get_possible_move_list();

                    if (not list.empty()) {
                        std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);

                        engine->move(list[distribution(_rng)]);
                    }
                }

                void simulate_one_game_from_root() {
                    Node<Decision> *current = descent();

                    if (current != nullptr and current->get_unvisited_child_number() > 0) {
                        openxum::core::common::Engine<Decision> *currentEngine;

                        if (not current->engine()->is_finished()) {
                            openxum::ai::common::Node<Decision> *child = current->get_next_unvisited_child();

                            // add_children(child);
                            currentEngine = child->engine();
                            current = child;
                        } else {
                            currentEngine = current->engine();
                        }

                        openxum::core::common::Engine<Decision> *engine = currentEngine->clone();

                        int winner;
                        unsigned int distance = 0;

                        evaluate(currentEngine, engine, winner, distance);
                        updateScore(current, winner, *engine);
                        updateDistance(current, distance);
                        delete engine;
                    }
                }

                void updateDistance(Node<Decision> *current, unsigned int distance) {
                    if (current != nullptr) {
                        if (current->get_next_goal_distance() < 0 or current->get_next_goal_distance() < distance) {
                            current->set_next_goal_distance(distance);
                            current = current->get_father();
                            while (current != nullptr) {
                                ++distance;
                                if (current->get_next_goal_distance() > distance) {
                                    current->set_next_goal_distance(distance);
                                    current = current->get_father();
                                } else {
                                    break;
                                }
                            }
                        }
                    }
                }

                void updateScore(Node<Decision> *current,
                                 int winner,
                                 const openxum::core::common::Engine<Decision> &engine) {
                    while (current != nullptr) {
                        current->visit();
                        if (winner == this->color()) {
                            current->inc_wins(engine.gain(this->color(), true));
                        } else {
                            current->inc_losses(engine.gain(winner, true));
                        }
                        current = current->get_father();
                    }
                }

                unsigned int _simulation_number;
                bool _stoppable;
                unsigned int _move_number;
                unsigned int _new_move_number;
                unsigned int _evaluation_number;
                Node<Decision> *_root;
                std::random_device _random_device;
                std::mt19937 _rng;
                std::map<std::string, Node<Decision> *> _nodes;
                Node<Decision> *_best;
                double _next_goal_distance;
                unsigned int _next_goal_distance_evaluation;
            };

        }
    }
}

#endif