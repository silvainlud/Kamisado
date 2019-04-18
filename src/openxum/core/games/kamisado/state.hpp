#ifndef KAMISADO_STATE_H
#define KAMISADO_STATE_H


#include "coordinates.hpp"
#include "enum_utils.h"

class State {

public:
    State();

    int _x;
    int _y;
    int _color;

    State(int , int, int );

};


#endif //KAMISADO_STATE_H
