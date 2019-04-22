#include <openxum/ai/specific/kamisado/random_player.hpp>

namespace openxum {
    namespace ai {
        namespace specific {
            namespace kamisado {

                openxum::core::common::Move* RandomPlayer::get_move()
                {
                    const openxum::core::common::Moves& list = engine().get_possible_move_list();

                    if (not list.empty()) {
                        std::mt19937 rng(8372);
                        std::uniform_int_distribution<std::mt19937::result_type> distribution(0, list.size());

                        return list[distribution(rng)];
                    } else {
                        return nullptr;
                    }
                }

            }
        }
    }
}