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

Node::Node(openxum::core::common::Engine *engine, Node *father, openxum::core::common::Move *move)
    : _engine(engine), _father(father), _move(move), _loss_number(0),
      _visit_number(0), _win_number(0), _depth(0), _max_depth(0), _next_gloal_distance(-1)
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

Node::~Node()
{
  delete _engine;
  delete _move;
  for (auto c: _children) {
    delete c;
  }
}

void Node::add_children(Node *n)
{
  _children.push_back(n);
}

Node *Node::choice()
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

double Node::compute_score() const
{
  double exploitation = _visit_number == 0 ? 0 : _win_number / _visit_number;
  double exploration = sqrt(2 * log(double(_father->get_visit_number()) / _visit_number));

  return exploitation + 5 * KUCT * exploration;
}

openxum::core::common::Engine *Node::engine() const
{
  return _engine;
}

Node *Node::get_father() const
{
  return _father;
}

int Node::get_level() const
{
  return _level;
}

openxum::core::common::Move *Node::get_move() const
{
  return _move;
}

double Node::get_number_of_wins() const
{
  return _win_number;
}

double Node::get_number_of_losses() const
{
  return _loss_number;
}

int Node::get_visit_number() const
{
  return _visit_number;
}

void Node::inc_wins(double delta)
{
  _win_number += delta;
}

void Node::inc_losses(double delta)
{
  _loss_number += delta;
}

bool Node::is_finished() const
{
  return _engine->is_finished();
}

void Node::visit()
{
  ++_visit_number;
}

openxum::ai::common::Node *Node::get_next_unvisited_child()
{
  Node *current = this;
  size_t index = _possible_moves.size() - _unvisited_child_number;

  --_unvisited_child_number;
  while (current != nullptr) {
    --current->_possible_move_number;
    current = current->get_father();
  }
  _children.push_back(new Node(_engine->clone(), this, _possible_moves[index]->clone()));
  return _children.back();
}

}
}
}