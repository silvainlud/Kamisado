#include "RandomPlayer.h"



RandomPlayer::RandomPlayer(Engine engine, int color) : _engine(std::move(engine)), _color(color)
{

}

void RandomPlayer::play_a_random_turn(Engine engine) {

    std::vector<State> towers = engine.get_current_towers();
    State chosenTower;
    Coordinates chosenMove;
    std::vector<Coordinates> possibleMoves;

    if(engine._play_color == NONE)
    {

        std::mt19937 rng;
        rng.seed(static_cast<unsigned int>(time(nullptr)));
        std::uniform_int_distribution<std::mt19937::result_type> distribution(0,towers.size()-1);
        chosenTower = towers[distribution(rng)];

    }
    else
    {

        chosenTower._color = engine._play_color;
        Coordinates tmpCoord =  engine.find_playable_tower(engine._play_color);
        chosenTower._x = tmpCoord._x;
        chosenTower._y = tmpCoord._y;

    }

    std::vector<Coordinates> moveList = engine.get_possible_moving_list(chosenTower);

    std::mt19937 rng2;
    rng2.seed(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,moveList.size()-1);
    chosenMove = moveList[distribution(rng2)];

    engine.move(Move(Coordinates(chosenTower._x, chosenTower._y), chosenMove));

}