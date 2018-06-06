#ifndef KAMISADO_RANDOMPLAYER_H
#define KAMISADO_RANDOMPLAYER_H

#include <random>
#include <iostream>
#include <ctime>
#include <utility>
#include "Engine.h"
#include "Node.h"


class RandomPlayer {
private:
    Engine _engine;
    int _color;

public:
    RandomPlayer(Engine engine , int color );

    void play_a_random_turn(Engine);
};


#endif //KAMISADO_RANDOMPLAYER_H
