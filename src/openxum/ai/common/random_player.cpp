#include <openxum/ai/common/random_player.hpp>

namespace openxum {
    namespace ai {
        namespace common {

            openxum::core::common::Move* RandomPlayer::get_move()
            {
                const openxum::core::common::Moves& list = engine().get_possible_move_list();

                if (not list.empty()) {
                    std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size() - 1);

                    return list[distribution(_rng)];
                } else {
                    return nullptr;
                }
            }

        }
    }
}