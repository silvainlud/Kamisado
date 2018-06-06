#ifndef KAMISADO_MOVE_H
#define KAMISADO_MOVE_H


#include "Coordinates.h"

class Move {
public:
    Coordinates _from;
    Coordinates _to;
    Move(Coordinates , Coordinates );
};


#endif //KAMISADO_MOVE_H
