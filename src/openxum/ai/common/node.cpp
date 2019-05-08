/**
 * @file openxum/ai/common/node.hpp
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

#include <openxum/ai/common/node.hpp>
#include <cmath>
#include <random>

#define KUCT 0.25

namespace openxum {
    namespace ai {
        namespace common {

            Node::Node(openxum::core::common::Engine* engine, Node* father, openxum::core::common::Move* move)
                    :_engine(engine), _father(father), _move(move), _lossNumber(0),
                     _visitNumber(0), _winNumber(0)
            {
                if (father != nullptr) {
                    _level = _father->_level + 1;
                } else {
                    _level = 0;
                }
                if (move != nullptr) {
                    _engine->move(move);
                }
                _possible_moves = _engine->get_possible_move_list();
                _unvisited_child_number = (unsigned int) _possible_moves.size();

                // possible moves in sub tree with this as root
                _possible_move_number = _unvisited_child_number;

                Node* current = _father;

                while (current != nullptr) {
                    current->_possible_move_number += _possible_move_number;
                    current = current->get_father();
                }
            }

            Node::~Node()
            {
                delete _engine;
                delete _move;
            }

            void Node::add_children(Node* n)
            {
                _children.push_back(n);
            }

            Node* Node::choice()
            {
                if (_children.empty()) {
                    return nullptr;
                } else {
//                    Node* best = nullptr;
//                    double bestScore = -1;
//
//                    for (Node* child: _children) {
//                        double score = child->compute_score(is_current);
//
//                        if (score > bestScore) {
//                            bestScore = score;
//                            best = child;
//                        }
//                    }
//                    return best;

                    double sum = 0;
                    std::vector<double> scores;

                    for (Node* child: _children) {
                        double score = child->compute_score();

                        scores.push_back(score);
                        sum += score;
                    }

                    std::random_device random_device;
                    std::mt19937 rng(random_device());
                    std::uniform_real_distribution<double> distribution(0, sum);
                    double value = distribution(rng);
                    bool found = false;
                    std::vector<double>::size_type index = 0;

                    sum = 0;
                    while (not found and index < scores.size()) {
                        sum += scores[index];
                        found = sum > value;
                        if (not found) {
                            ++index;
                        }
                    }
                    if (found) {
                        return _children[index];
                    } else {
                        return _children[_children.size() - 1];
                    }
                }
            }

            double Node::compute_score() const
            {
                double exploitation = double(_winNumber) / _visitNumber;

//                double exploration = sqrt(2 * log(double(_father->get_visit_number()) / _visitNumber));
                double exploration = _father->get_possible_move_number() == 0 ? 0 : double(_possible_move_number)
                        / _father->get_possible_move_number();

                return exploitation + KUCT * exploration;
            }

            openxum::core::common::Engine* Node::engine() const
            {
                return _engine;
            }

            Node* Node::get_father() const
            {
                return _father;
            }

            int Node::get_level() const
            {
                return _level;
            }

            openxum::core::common::Move* Node::get_move() const
            {
                return _move;
            }

            int Node::get_number_of_wins() const
            {
                return _winNumber;
            }

            int Node::get_number_of_losses() const
            {
                return _lossNumber;
            }

            int Node::get_visit_number() const
            {
                return _visitNumber;
            }

            void Node::inc_wins()
            {
                _winNumber++;
            }

            void Node::inc_losses()
            {
                _lossNumber++;
            }

            bool Node::is_finished() const
            {
                return _engine->is_finished();
            }

            void Node::visit()
            {
                ++_visitNumber;
            }

            openxum::ai::common::Node* Node::get_first_unvisited_child()
            {
                Node* current = this;

                --_unvisited_child_number;
                while (current != nullptr) {
                    -- current->_possible_move_number;
                    current = current->get_father();
                }
                return _children[_possible_moves.size() - _unvisited_child_number - 1];
            }

        }
    }
}