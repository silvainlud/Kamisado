#include "Node.h"
#include <cmath>
#include <cstdlib>

#define KUCT 0.25

void Node::add_children(Node n) {
    _childNodes.push_back(n);
}

Node Node::choice(bool max) {
   std::vector<Node> tmpChildren =  get_child_nodes();
    if(tmpChildren.empty())
        exit(-1); // TODO trouver mieux
    else
    {
        Node best = tmpChildren[0];
        double bestScore = best.compute_score();
        double score = 0;
        for(Node i : tmpChildren)
        {
            score = i.compute_score();

            if(( max && score < bestScore) || (!max  && score < bestScore))
            {
                bestScore = score;
                best = i;
            }
        }
        return best;
    }

}

double Node::compute_score() {
   double exploration = _winNumber / _visitNumber;
    double exploitation = sqrt(2*log(_father->get_visit_number() / _visitNumber));
    return exploitation + KUCT * exploration;
}

std::vector<Node> Node::get_child_nodes() {
    return _childNodes;

}

Engine Node::get_engine() {
    return _engine;
}

Node* Node::get_father() {
    return _father;
}

Move Node::get_first_possible_move() {
    return _possibleMoves[0];
}

int Node::get_level() {
    return _level;
}

Move Node::get_move() {
    return _move;
}

int Node::get_number_of_wins() {
    return _winNumber;
}

int Node::get_number_of_losses() {
    return _lossNumber;
}

std::vector<Move> Node::get_possible_moves() {
    return _possibleMoves;
}

int Node::get_visit_number() {
    return _visitNumber;
}

void Node::inc_wins() {
    _winNumber++;
}

void Node::inc_losses() {
    _lossNumber++;
}

bool Node::is_finished() {
    return _engine.is_finished();
}

void Node::remove_first_possible_move() {
    _possibleMoves.erase(_possibleMoves.begin());
}

void Node::visit() {
    ++_visitNumber;
}

Node::Node(Engine engine, Node* father, Move move, std::vector<Move> possibleMoves) : _engine(engine), _father(father), _move(move), _possibleMoves(possibleMoves), _winNumber(0), _lossNumber(0), _visitNumber(0) {
    if( father != nullptr)
    {
        _level = _father->_level +1;
    }
    else _level = 0;
}

Node::Node() {

}
