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

#ifndef OPENXUM_AI_SPECIFIC_KAMISADO_NODE_HPP
#define OPENXUM_AI_SPECIFIC_KAMISADO_NODE_HPP

#include <vector>
#include <openxum/core/games/kamisado/engine.hpp>
#include <openxum/core/games/kamisado/move.hpp>

namespace openxum {
    namespace ai {
        namespace specific {
            namespace kamisado {

                class Node {
                public:
                    Node(openxum::core::common::Engine*, Node*, openxum::core::common::Move*);

                    virtual ~Node();

                    void add_children(Node*);

                    Node* choice(bool);

                    double compute_score() const;

                    openxum::core::common::Engine* engine() const;

                    const std::vector<Node*>& get_children() const
                    {
                        return _children;
                    }

                    Node* get_father() const;

                    openxum::core::common::Move* get_first_possible_move() const;

                    int get_level() const;

                    openxum::core::common::Move* get_move() const;

                    int get_number_of_wins() const;

                    int get_number_of_losses() const;

                    const std::vector<openxum::core::common::Move*>& get_possible_moves() const;

                    int get_visit_number() const;

                    void inc_wins();

                    void inc_losses();

                    bool is_finished() const;

                    void remove_first_possible_move();

                    void visit();

                private:
                    int _level;
                    openxum::core::common::Engine* _engine;
                    Node* _father;
                    openxum::core::common::Move* _move;
                    int _lossNumber;
                    int _visitNumber;
                    int _winNumber;
                    std::vector<Node*> _children;
                    openxum::core::common::Moves _possible_moves;
                };

            }
        }
    }
}

#endif