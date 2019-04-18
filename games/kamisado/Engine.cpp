#include <iostream>
#include <ctime>
#include <random>
#include "Engine.h"

int Engine::current_color() {
    return _color;
}

std::vector<State> Engine::get_black_towers() {
    return _black_towers;
}

std::vector<State> Engine::get_white_towers() {
    return _white_towers;
}

int Engine::get_play_color() {
    return _play_color;
}

std::vector<State> Engine::get_towers(int color) {
    std::vector<State> result;
    if(_color == WHITE)
      result = get_white_towers();
    else result = get_black_towers();
    return result;
}

int Engine::next_color(int color) {
    if(_color == WHITE)
     return BLACK;
    else return WHITE;
}

void Engine::change_color() {
    _color = next_color(_color);
}

bool Engine::is_finished() {
    return (_phase == FINISH);
}

int Engine::winner_is() {
    if(is_finished())
        return _color;
    else return NONE;
}

int Engine::get_phase() {
    return _phase;
}

std::vector<State> Engine::get_current_towers() {
    std::vector<State> result;
    if(_color == BLACK)
       result = get_black_towers();
    else result = get_white_towers();
    return result;
}

void Engine::move(Move movement) {
 move_tower(movement._from, movement._to);
}



Engine::Engine(int type, int color) : _type(type), _color(color) {

   for(int i= 0; i < 8; i++)
   {
       State blacks(i, 0, _colors[i][0]);
       _black_towers.push_back(blacks);
   }
    for(int i= 0; i < 8; i++)
    {

        State whites(i, 7, _colors[i][7]);
        _white_towers.push_back(whites);
    }

    _phase = MOVE_TOWER;
    _play_color = NONE;
}

void Engine::move_tower(Coordinates selected_tower, Coordinates selected_cell) {
    State tower = find_towers2(selected_tower, _color);
    if(tower._x != -1)
    {
        tower._x = selected_cell._x;
        tower._y = selected_cell._x;
    }
    if(_color == BLACK && tower._y == 7 || _color == WHITE && tower._y == 0)
        _phase = FINISH;
    else
    {
        _play_color = _colors[tower._x][tower._y];
        if(!pass(next_color(_color)))
        {
            change_color();
        }
        else
        {
            Coordinates playable_tower = find_playable_tower(next_color(_color));
            _play_color = _colors[playable_tower._x][playable_tower._y];

            if(pass(_color))
            {
                _phase = FINISH;
                _color = next_color(_color);
            }
        }
    }
}

State Engine::find_tower(Coordinates coord, int color) {
    State result = find_towers2(coord, color);
    return result;
}

bool Engine::pass(int color) {
    Coordinates coord = find_playable_tower(color);
    State state(coord._x, coord._y, color);
    std::vector<Coordinates> moveList = get_possible_moving_list(state);

    return !moveList.empty();
}


std::vector<Coordinates> Engine::get_possible_moving_list(State tower) {
    std::vector<Coordinates> moveList;
    int step;
    int limit;

    bool player_color = is_black(tower);

    if(player_color)
    {
        step = 1;
        limit = 8;
    }
    else
    {
        step = -1;
        limit = -1;
    }
    //colonne
    int line = tower._y + step;

    Coordinates coord(tower._x, line);

    while(line != limit  && !is_empty(coord))
    {
        moveList.push_back(coord);
        line+=step;
        coord._y+=step;

    }


    int col = tower._x + 1;
    line = tower._y + step;

    Coordinates coord2(col, line);
    while(line != limit  && col!=8 && !is_empty(coord))
    {
        moveList.push_back(coord2);
        line+=step;

        coord2._y+=step;

        coord2._x++;

    }


    col = tower._x - 1;
    line = tower._y + step;

    Coordinates coord3(col, line);
    while(line != limit  && col!=-1 && !is_empty(coord))
    {
        moveList.push_back(coord3);
        line+=step;
        coord3._y+=step;
        coord3._x--;
    }


    return moveList;
}

bool Engine::is_empty(Coordinates coord) {
    bool found = false;
    int i =0;

    while(i<8 && !found)
    {
        if(_black_towers[i]._x == coord._x && _black_towers[i]._y == coord._y){
            found = true;
        }
        else i++;
    }
    i=0;
    while(i<8 && !found)
    {
        if(_white_towers[i]._x == coord._x && _white_towers[i]._y == coord._y){
            found = true;
        }
        else i++;
    }
    return found;
}

State Engine::find_towers2(Coordinates coord, int color) {
    State result;
    std::vector<State> towers = get_towers(color);
    bool found = false;
    int i = 0;

    while(i<8 && !found)
    {
        if(towers[i]._x == coord._x && towers[i]._y == coord._y )
        {
            result = towers[i];
            found = true;
        }
        else i++;
    }
    if(!found)
       result._x = -1; // indicates nonexistence
    return result;
}

Coordinates Engine::find_playable_tower(int color) {

    Coordinates result;
    std::vector<State> towers = get_towers(color);

    if(_play_color == NONE)
    {
        result._x = result._y = -1;
    }
    else
    {
        for ( State i : towers)
        {
            if(i._color == color)
            {
                result._x = i._x;
                result._y = i._y;
            }

        }
    }
    return result;
}

Engine::Engine() {}

std::vector<Move> Engine::get_possible_move_list() {
    Coordinates tower = find_playable_tower(_play_color);
    std::vector<Coordinates> coordList;
    std::vector<Move> moveList;
    if(tower._x == -1)
    {
        std::vector<State> towers = get_current_towers();
        State chosenTower;

        std::mt19937 rng;
        rng.seed(static_cast<unsigned int>(time(nullptr)));
        std::uniform_int_distribution<std::mt19937::result_type> distribution(0,towers.size()-1);
        chosenTower = towers[distribution(rng)];

        coordList = get_possible_moving_list(State(chosenTower._x, chosenTower._y, _play_color));


    }
    else
    {
        coordList = get_possible_moving_list(State(tower._x, tower._y, _play_color));

        for(Coordinates i : coordList)
        {
            moveList.emplace_back(tower, i);
        }
    }

    return moveList;
}

bool Engine::is_black(State tower) {
    bool found = false;
    int i =0;

    while(i<8 && !found)
    {
        if(_black_towers[i]._x == tower._x && _black_towers[i]._y == tower._y){
            found = true;
        }
        else i++;
    }
}


