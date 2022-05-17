//
// Created by mithrandir on 17/05/22.
//

#ifndef OPENXUM_WS_CPP_LAMBDA_MCTS_HPP
#define OPENXUM_WS_CPP_LAMBDA_MCTS_HPP

#include <openxum/ai/common/mcts_player.hpp>
#include <openxum/core/games/kamisado/decision.hpp>

namespace openxum {
    namespace ai {
        namespace specific {
            namespace kamisado {

                class LambdaMcts : public openxum::ai::common::MCTSPlayer<core::games::kamisado::Decision> {
                private:
                    /**
                     * @var int représente le nombre de coups avant une vérification
                     */
                    int beta;
                    int counter = 0;
                    double lambda;
                    std::vector<double> scores;
                public:
                    LambdaMcts(int color, int opponent, int epsilon, int beta, double lambda,
                               openxum::core::common::Engine<core::games::kamisado::Decision> *e)
                            : openxum::ai::common::MCTSPlayer<core::games::kamisado::Decision>(color, opponent, e,
                                                                                               epsilon), beta(beta),
                              lambda(lambda) {}


                    openxum::core::common::Move<core::games::kamisado::Decision> get_move() override {
                        openxum::ai::common::MCTSPlayer<core::games::kamisado::Decision>::get_move();

                        if (counter > beta) {
                            counter = 0;

                            double score = get_final_choice()->compute_score();
                            scores.push_back(score);

                            if (score > lambda) {
                                std::cout << "Level need to change ! DOWN " << std::endl;
                                //Too high
                                _simulation_number = (_simulation_number == 1) ? 1 : (_simulation_number - 1);
                            } else if(score < -lambda) {
                                std::cout << "Level need to change ! UP " << std::endl;
                                _simulation_number = (_simulation_number == 9) ? 9 : (_simulation_number + 1);
                            }

                        }


                    }
                };


            }
        }
    }
}
#endif //OPENXUM_WS_CPP_LAMBDA_MCTS_HPP
