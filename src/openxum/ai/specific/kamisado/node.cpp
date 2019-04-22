/**
 * @file openxum/core/common/node.hpp
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

#include <openxum/ai/specific/kamisado/node.hpp>
#include <cmath>

#define KUCT 0.25

namespace openxum {
    namespace ai {
        namespace specific {
            namespace kamisado {

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
                }

                Node::~Node()
                {
                    delete _engine;
                    delete _move;
                    for (Node* child: _children) {
                        delete child;
                    }
                }

                void Node::add_children(Node* n)
                {
                    _children.push_back(n);
                }

                Node* Node::choice(bool max)
                {
                    if (_children.empty()) {
                        return nullptr;
                    } else {
                        Node* best = nullptr;
                        double bestScore = -1;

                        for (Node* child: _children) {
                            double score = child->compute_score();

                            if ((max and score > bestScore) or (not max and score < bestScore)) {
                                bestScore = score;
                                best = child;
                            }
                        }
                        return best;
                    }
                }

                double Node::compute_score() const
                {
                    double exploration = double(_winNumber) / _visitNumber;
                    double exploitation = sqrt(2 * log(double(_father->get_visit_number()) / _visitNumber));

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

                openxum::core::common::Move* Node::get_first_possible_move() const
                {
                    return *_possible_moves.begin();
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

                const std::vector<openxum::core::common::Move*>& Node::get_possible_moves() const
                {
                    return _possible_moves;
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

                void Node::remove_first_possible_move()
                {
                    openxum::core::common::Move* move = *_possible_moves.begin();

                    delete move;
                    _possible_moves.erase(_possible_moves.begin());
                }

                void Node::visit()
                {
                    ++_visitNumber;
                }

            }
        }
    }
}