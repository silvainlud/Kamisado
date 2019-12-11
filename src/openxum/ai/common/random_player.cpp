#include <openxum/ai/common/random_player.hpp>

namespace openxum {
namespace ai {
namespace common {

openxum::core::common::Move *RandomPlayer::get_move()
{
  const openxum::core::common::Moves &list = engine().get_possible_move_list();
  openxum::core::common::Move *move = nullptr;

  if (not list.empty()) {
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);

    move = list[distribution(_rng)]->clone();
    compute_next_goal_distance(move);
  }
  return move;
}

unsigned int RandomPlayer::compute_next_goal_distance(const openxum::core::common::Move *move)
{
  openxum::core::common::Engine *e = engine().clone();

  e->move(move);

  unsigned int initial_goal = e->current_goal(color());

  _next_goal_distance = 0;
  while (not e->is_finished() and e->current_goal(color()) == initial_goal) {
    const openxum::core::common::Moves &list = e->get_possible_move_list();

    if (not list.empty()) {
      std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);

      e->move(list[distribution(_rng)]);
      ++_next_goal_distance;
    }
  }
  delete e;
}

}
}
}