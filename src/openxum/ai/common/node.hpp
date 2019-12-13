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

#include <openxum/core/common/engine.hpp>
#include <openxum/core/common/move.hpp>

#include <cmath>
#include <random>
#include <vector>

namespace openxum {
namespace ai {
namespace common {

#define KUCT 0.25

template<class Decision>
class Node
{
public:
  Node(openxum::core::common::Engine<Decision> *engine,
       Node *father,
       const openxum::core::common::Move<Decision>& move)
      : _engine(engine), _father(father), _move(move), _loss_number(0),
        _visit_number(0), _win_number(0), _depth(0), _max_depth(0), _next_gloal_distance(-1)
  {
    if (father != nullptr) {
      _level = _father->_level + 1;
    } else {
      _level = 0;
    }
    if (not move.empty()) {
      _engine->move(move);
    }
    _possible_moves = _engine->get_possible_move_list();
    _unvisited_child_number = (unsigned int) _possible_moves.size();

    // possible moves in sub tree with this as root
    _possible_move_number = _unvisited_child_number;

    Node *current = _father;

    if (_father != nullptr) {
      _depth = _father->depth() + 1;
      _max_depth = _depth;
    }
    while (current != nullptr) {
      current->_possible_move_number += _possible_move_number;
      if (current->_max_depth < _max_depth) {
        current->_max_depth = _max_depth;
      }
      current = current->get_father();
    }
  }

  virtual ~Node()
  {
    delete _engine;
    for (auto c: _children) {
      delete c;
    }
  }

  Node *choice()
  {
    if (_children.empty()) {
      return nullptr;
    } else {
      Node *best = nullptr;
      double bestScore = -1;

      for (Node *child: _children) {
        double score = child->compute_score();

        if (score > bestScore) {
          bestScore = score;
          best = child;
        }
      }
      return best;
    }
  }

  double compute_score() const
  {
    double exploitation = _visit_number == 0 ? 0 : _win_number / _visit_number;
    double exploration = sqrt(2 * log(double(_father->get_visit_number()) / _visit_number));

    return exploitation + 5 * KUCT * exploration;
  }

  unsigned int depth() const
  { return _depth; }

  openxum::core::common::Engine<Decision> *engine() const
  { return _engine; }

  const std::vector<Node *> &get_children() const
  { return _children; }

  Node *get_father() const
  { return _father; }

  int get_level() const
  { return _level; }

  const openxum::core::common::Move<Decision>& get_move() const
  { return _move; }

  double get_next_goal_distance() const
  { return _next_gloal_distance; }

  openxum::ai::common::Node<Decision> *get_next_unvisited_child()
  {
    Node *current = this;
    size_t index = _possible_moves.size() - _unvisited_child_number;

    --_unvisited_child_number;
    while (current != nullptr) {
      --current->_possible_move_number;
      current = current->get_father();
    }
    _children.push_back(new Node(_engine->clone(), this, _possible_moves[index]));
    return _children.back();
  }

  double get_number_of_wins() const
  { return _win_number; }

  double get_number_of_losses() const
  { return _loss_number; }

  typename openxum::core::common::Moves<Decision>::size_type
  get_possible_move_number() const
  { return _possible_move_number; }

  const openxum::core::common::Moves<Decision> &get_possible_moves() const
  { return _possible_moves; }

  int get_visit_number() const
  { return _visit_number; }

  std::string id() const
  { return _engine->id(); }

  void inc_wins(double delta)
  { _win_number += delta; }

  void inc_losses(double delta)
  { _loss_number += delta; }

  bool is_finished() const
  { return _engine->is_finished(); }

  unsigned int get_unvisited_child_number() const
  { return _unvisited_child_number; }

  unsigned int max_depth() const
  { return _max_depth; }

  void set_next_goal_distance(double distance)
  { _next_gloal_distance = distance; }

  void visit()
  { ++_visit_number; }

private:
  int _level;
  openxum::core::common::Engine<Decision> *_engine;
  Node *_father;
  std::vector<Node *> _children;
  openxum::core::common::Move<Decision> _move;
  openxum::core::common::Moves<Decision> _possible_moves;
  double _loss_number;
  int _visit_number;
  double _win_number;
  unsigned int _unvisited_child_number;
  unsigned int _possible_move_number;
  unsigned int _depth;
  unsigned int _max_depth;
  double _next_gloal_distance;
};

}
}
}

#endif