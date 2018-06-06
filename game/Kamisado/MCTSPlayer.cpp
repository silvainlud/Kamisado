#include "MCTSPlayer.h"



MCTSPlayer::MCTSPlayer(Engine engine, int color, int simulation_number) {
    _engine = std::move(engine);
    _color = color;
    _root = Node();
    _simulation_number = simulation_number;

}


MCTSPlayer::MCTSPlayer() = default;


Move MCTSPlayer::move() {

    init_search();
    for(int i = 0; i< _simulation_number; i++)
    {
        simulate_one_game_from_root();
    }
    return get_final_choice();
}

int MCTSPlayer::evaluate(Engine engine) {
    Engine tmpEngine = std::move(engine);

    while(!tmpEngine.is_finished())
    {
        play_a_random_turn(tmpEngine);
    }

    return tmpEngine.winner_is();
}

void MCTSPlayer::init_search() {

    _root._engine = _engine;
   // _root._possibleMoves = _engine.get_possible_moving_list(_color);
}

void MCTSPlayer::simulate_one_game_from_root() {
    Node current = _root;
    Node node = current;
    Engine tmpEngine;

    //descent
    while(&current != nullptr && !current.get_engine().is_finished())
    {
       std::vector<Move> possibleMoves = current.get_possible_moves();

        if(!possibleMoves.empty())
        {
            node = current;
            break;
        }
        else
        {
            node = current;
            bool condition = current.get_engine().current_color() == _color;
            current = current.choice(condition);
        }
    }

    //expansion

    if (&current == nullptr || !current.get_engine().is_finished() )
    {
        current = node;
        Engine newEngine = current.get_engine() ;
        Move move = current.get_first_possible_move();
        newEngine.move(move);
        Coordinates tmpCoord = newEngine.find_playable_tower(_color);
        Node * addrNode = &node;

        Node newNode(newEngine, addrNode, move, newEngine.get_possible_move_list() );
        current.remove_first_possible_move();
        current.add_children(newNode);
        tmpEngine = newNode.get_engine();
        current = newNode;
    }
    else
    {
        tmpEngine = current.get_engine();
    }

    // evaluation
    int winner = evaluate(tmpEngine);

    //update
    updateScore(current, winner);

}

void MCTSPlayer::updateScore(Node current, int winner) {
    while(&current != nullptr)
    {
        current.visit();
        if(winner == _color)
            current.inc_wins();
        else current.inc_losses();

        current = *current.get_father();
    }
}

Move MCTSPlayer::get_final_choice() {
    Move finalChoice = _root.get_child_nodes()[0].get_move();
    int best = _root.get_child_nodes()[0].get_visit_number();

    for(Node i : _root.get_child_nodes())
    {
        if(i.get_visit_number() > best)
        {
            best = i.get_visit_number();
            finalChoice = i.get_move();
        }
    }
    return finalChoice;
}


void MCTSPlayer::play_a_random_turn(Engine engine) {
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

void MCTSPlayer::movement(Engine engine)
{
    _engine = engine;
    Move tmpMove =  move();
    engine.move(tmpMove);
}


