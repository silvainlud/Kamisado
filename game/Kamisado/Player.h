#ifndef KAMISADO_PLAYER_H
#define KAMISADO_PLAYER_H


#include "Engine.h"
#include "Node.h"

 class Player {
protected:
    int _color;
    Engine _engine;


public:
    Player();
    Player(Engine, int);
     Move move(); // TODO à retirer


     virtual int evaluate(Engine engine);  // TODO à retirer
 };


#endif //KAMISADO_PLAYER_H
