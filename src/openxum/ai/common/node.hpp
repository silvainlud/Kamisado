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

#ifndef OPENXUM_AI_COMMON_NODE_HPP
#define OPENXUM_AI_COMMON_NODE_HPP

#include <vector>
#include <openxum/core/common/engine.hpp>
#include <openxum/core/common/move.hpp>

namespace openxum {
    namespace ai {
        namespace common {

            class Node {
            public:
                Node(openxum::core::common::Engine*, Node*, openxum::core::common::Move*);

                virtual ~Node();

                void add_children(Node*);

                Node* choice();

                double compute_score() const;

                unsigned int depth() const
                { return _depth; }

                openxum::core::common::Engine* engine() const;

                const std::vector<Node*>& get_children() const { return _children; }

                Node* get_father() const;

                int get_level() const;

                openxum::core::common::Move* get_move() const;

                int get_number_of_wins() const;

                int get_number_of_losses() const;

                openxum::core::common::Moves::size_type
                get_possible_move_number() const { return _possible_move_number; }

                const openxum::core::common::Moves& get_possible_moves() const
                { return _possible_moves; }

                int get_visit_number() const;

                std::string id() const { return _engine->id(); }

                void inc_wins();

                void inc_losses();

                bool is_finished() const;

                unsigned int get_unvisited_child_number() const
                { return _unvisited_child_number; }

                unsigned int max_depth() const
                { return _max_depth; }

                void visit();

                openxum::ai::common::Node* get_next_unvisited_child();

            private:
                int _level;
                openxum::core::common::Engine* _engine;
                Node* _father;
                std::vector<Node*> _children;
                openxum::core::common::Move* _move;
                openxum::core::common::Moves _possible_moves;
                int _loss_number;
                int _visit_number;
                int _win_number;
                unsigned int _unvisited_child_number;
                unsigned int _possible_move_number;
                unsigned int _depth;
                unsigned int _max_depth;
            };

        }
    }
}

#endif