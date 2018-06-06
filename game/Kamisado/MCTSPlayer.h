#ifndef KAMISADO_MCTSPLAYER_H
#define KAMISADO_MCTSPLAYER_H

#include "Engine.h"
#include "Node.h"
#include <utility>
#include <random>
#include <ctime>

class MCTSPlayer {
protected:
    int  _color;
    int _simulation_number;
    Node _root;

    MCTSPlayer();
    int evaluate(Engine);
    Move get_final_choice();

    void play_a_random_turn(Engine);
    void simulate_one_game_from_root();
    void updateScore(Node, int);


public:
    MCTSPlayer(Engine, int, int);

    void init_search();

    Move move();

    Engine _engine;

    void movement(Engine engine);
};


#endif //KAMISADO_MCTSPLAYER_H
