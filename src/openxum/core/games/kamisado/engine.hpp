#ifndef OPENXUM_CORE_GAMES_KAMISADO_ENGINE_HPP
#define OPENXUM_CORE_GAMES_KAMISADO_ENGINE_HPP

#include <openxum/core/games/kamisado/coordinates.hpp>
#include <openxum/core/games/kamisado/state.hpp>
#include <openxum/core/games/kamisado/move.hpp>

#include <vector>

class Engine {
public:
    int _color; // BLACK ou WHITE, couleur du joueur
    int _type; // ENUM Type
    std::vector<State> _black_towers;
    std::vector<State> _white_towers;
    int _play_color; // ENUM COLORS
    int _phase;  //ENUM PHASE
    bool is_black(State);

    Engine();
    Engine(int type, int color);

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

private:
    int next_color(int color);
    void change_color();
    void move_tower(Coordinates selected_tower, Coordinates selected_cell);
    State find_towers2(Coordinates, int) ;
};


#endif //KAMISADO_ENGINE_H
