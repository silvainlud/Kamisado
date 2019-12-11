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
openxum::core::common::Move *MCTSPlayer::get_move()
{
  openxum::core::common::Move *move = nullptr;

  _move_number = 0;
  _new_move_number = 0;
  _evaluation_number = 0;
  _best = nullptr;
  init_search();
  for (unsigned int i = 0; i < _simulation_number; ++i) {
    simulate_one_game_from_root();
  }
  _best = get_final_choice();
  move = _best->get_move()->clone();
  compute_next_goal_distance(move);
  clear();
  return move;
}

void MCTSPlayer::compute_next_goal_distance(const openxum::core::common::Move * /* move */)
{
  _next_goal_distance = _best->get_next_goal_distance();
}

// private methods
void MCTSPlayer::add_children(Node *current)
{
  if (not current->is_finished()) {
    const openxum::core::common::Moves &possible_moves = current->get_possible_moves();

    for (auto move: possible_moves) {
      Node *newNode = new Node(current->engine()->clone(), current, move->clone());
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
  delete _root;
  _root = nullptr;
  for (auto const &n: _nodes) {
    delete n.second;
  }
  _nodes.clear();
}

Node *MCTSPlayer::descent() const
{
  Node *current = _root;

  while (current != nullptr and not current->is_finished()
      and current->get_unvisited_child_number() == 0) {
    current = current->choice();
  }
  return current;
}

void MCTSPlayer::evaluate(const openxum::core::common::Engine *engine,
                          openxum::core::common::Engine *clone, int &winner, unsigned int &distance)
{
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
      if (engine->current_goal(color()) == clone->current_goal(color())) {
        ++distance;
      }
      play_a_turn_randomly(clone);
    }
    winner = clone->winner_is();
  }
  _move_number += clone->move_number();
  _new_move_number += clone->move_number() - engine->move_number();
}

openxum::ai::common::Node *MCTSPlayer::get_final_choice()
{
  openxum::ai::common::Node *finalChoice = nullptr;
  double best = -1;

  for (Node *child: _root->get_children()) {
    double score = child->get_visit_number() == 0 ? 0 : child->get_number_of_wins()
        / child->get_visit_number();

    if (score > best) {
      best = score;
      finalChoice = child;
    }
  }
  return finalChoice;
}

void MCTSPlayer::init_search()
{
  _root = new Node(engine().clone(), nullptr, nullptr);
}

void MCTSPlayer::play_a_turn_randomly(openxum::core::common::Engine *engine)
{
  const openxum::core::common::Moves &list = engine->get_possible_move_list();

  if (not list.empty()) {
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);
    unsigned long index = distribution(_rng);
    openxum::core::common::Move *move = list[index];

    engine->move(move);
  }
}

void MCTSPlayer::simulate_one_game_from_root()
{
  Node *current = descent();

  if (current != nullptr and current->get_unvisited_child_number() > 0) {
    openxum::core::common::Engine *currentEngine;

    if (not current->engine()->is_finished()) {
      openxum::ai::common::Node *child = current->get_next_unvisited_child();

      // add_children(child);
      currentEngine = child->engine();
      current = child;
    } else {
      currentEngine = current->engine();
    }

    openxum::core::common::Engine *engine = currentEngine->clone();

    int winner;
    unsigned int distance = 0;

    evaluate(currentEngine, engine, winner, distance);
    updateScore(current, winner, *engine);
    updateDistance(current, distance);
    delete engine;
  }
}

void MCTSPlayer::updateDistance(Node *current, unsigned int distance)
{
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

void MCTSPlayer::updateScore(Node *current, int winner, const openxum::core::common::Engine &engine)
{
  while (current != nullptr) {
    current->visit();
    if (winner == color()) {
      current->inc_wins(engine.gain(color()));
    } else {
      current->inc_losses(engine.gain(winner));
    }
    current = current->get_father();
  }
}

}
}
}