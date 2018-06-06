#ifndef MCTS_NODE_H
#define MCTS_NODE_H

#include <vector>
#include "Engine.h"


class Node {

private:
    int _level;
    Node* _father;
    Move _move = Move(Coordinates(), Coordinates());
    int _visitNumber;
    int _lossNumber;
    int _winNumber;
    std::vector<Node> _childNodes;

public:
    Node(Engine, Node*, Move, std::vector<Move>);
    void add_children(Node);
    Node choice(bool);
    double compute_score();
    std::vector<Node> get_child_nodes();
    Engine get_engine();
    Node* get_father();
    Move get_first_possible_move();
    int  get_level();
    Move get_move();
    int  get_number_of_wins();
    int get_number_of_losses();
    std::vector<Move> get_possible_moves();
    int get_visit_number();
    void inc_wins();
    void inc_losses();
    bool is_finished();
    void remove_first_possible_move();
    void visit();

    Node();

    Engine _engine = Engine(0, 0);
    std::vector<Move> _possibleMoves;
};


#endif //MCTS_NODE_H
