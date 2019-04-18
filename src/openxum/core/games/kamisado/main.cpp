#include <iostream>

#include "engine.hpp"
#include "openxum/ai/specific/kamisado/RandomPlayer.h"
#include "openxum/ai/specific/kamisado/MCTSPlayer.h"



int main() {

    // Main temporaire (tests)

    Engine engine(SIMPLE, BLACK);
    RandomPlayer rdPlayer(engine, BLACK);
    RandomPlayer rdPlayer2(engine, WHITE);
    MCTSPlayer mctsPlayer(engine, WHITE, 20);

    while(!engine.is_finished())
    {
        std::cout << "Tour de random 1 " << std::endl;
        rdPlayer.play_a_random_turn(engine);
        std::cout << "Tour de mcts" << std::endl;
       // rdPlayer2.play_a_random_turn(engine);
        mctsPlayer.movement(engine);
    }
    std::cout << engine.winner_is() << std::endl;
    return 0;
}