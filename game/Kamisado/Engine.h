#ifndef KAMISADO_ENGINE_H
#define KAMISADO_ENGINE_H

#include <vector>
#include "Coordinates.h"
#include "State.h"
#include "Move.h"
#include <vector>


class Engine {

private:

    int next_color(int color);
    void change_color();
    void move_tower(Coordinates selected_tower, Coordinates selected_cell);

    State find_towers2(Coordinates, int) ;

public:
    int _color; // BLACK ou WHITe, couleur du joueur
    int _type; // ENUM Type
    std::vector<State> _black_towers;
    std::vector<State> _white_towers;
    int _play_color; // ENUM COLORS
    int _phase;  //ENUM PHASE
    bool is_black(State);


    Engine(int type, int color);

    Engine();

    int  current_color();
    std::vector<State> get_towers(int color);
    std::vector<State> get_black_towers();
    std::vector<State> get_white_towers();
    std::vector<State> get_current_towers();
    bool is_finished();
    int get_play_color();
    int winner_is();
    int get_phase();
    void move(Move);
    State find_tower(Coordinates, int);
    bool pass(int);
    std::vector<Coordinates>  get_possible_moving_list(State);
    std::vector<Move> get_possible_move_list();
    Coordinates find_playable_tower(int);


    int _colors[8][8] ={
            {ORANGE, BLUE, PURPLE, PINK, YELLOW, RED, GREEN, BROWN},
            {RED, ORANGE, PINK, GREEN, BLUE, YELLOW, BROWN, PURPLE},
            {GREEN, PINK, ORANGE, RED, PURPLE, BROWN, YELLOW, BLUE},
            {PINK, PURPLE, BLUE, ORANGE, BROWN, GREEN, RED, YELLOW},
            {YELLOW, RED,GREEN, BROWN, ORANGE, BLUE, PURPLE, PINK},
            {BLUE, YELLOW, BROWN, PURPLE, RED, ORANGE, PINK, GREEN},
            {PURPLE, BROWN, YELLOW, BLUE, GREEN, PINK, ORANGE, RED},
            {BROWN, GREEN, RED, YELLOW, PINK, PURPLE, BLUE, ORANGE}
    };

    bool is_empty(Coordinates);
};


#endif //KAMISADO_ENGINE_H
