/**
 * @file openxum/app/main.cpp
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2011-2019 Openxum Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <nlohmann/json.hpp>

#include <openxum/core/games/kamisado/engine.hpp>
#include <openxum/core/games/kamisado/game_type.hpp>
#include <openxum/ai/specific/kamisado/mcts_player.hpp>
#include <torch/script.h>

#define DEBUG 0
#define USE_LAMBDA 0
#define USE_AI 1

using json = nlohmann::json;

int beta = 5;
double lambda = 0.05;
int epsilon = 5;
int counter = 0;

const int maxLevelInHistory = 50;


double findScore(openxum::ai::specific::kamisado::MCTSPlayer *player, unsigned int level) {
    player->init_search();
    for (unsigned int i = 0; i < level; ++i) {
        player->simulate_one_game_from_root();
    }
    return player->get_final_choice()->compute_score();
}

int main(int, const char **) {

    std::vector<int> stats;

    const unsigned int partyNumber = 1000;
    const unsigned int multiplicator = 1;
#if USE_LAMBDA == 1 or USE_AI == 1
    std::cout << "id" << "," << "levelWhite" << "," << "levelBlack" << ","
              << "whiteWin"
              << "," << "blackWin"
              << "," << "countUp"
              << "," << "countDown"
              << "," << "countNothing"
              << "," << "countTotal"
              << "," << "epsilon"
              << std::endl;
#else

    std::cout << "id" << "," << "levelWhite" << "," << "levelBlack" << "," << "diff" << "," << "change" << ","
              << "score" << std::endl;
#endif

#if USE_AI == 1
    torch::jit::script::Module module;
    module = torch::jit::load("../../../model_white.pt");
    module.eval();
    module.train(false);
#endif

//    for (uint w = 1; w < 10; w++) {
    const uint levelWhite = 5;
//        for (uint j = 1; j < 10; j++) {
    const uint levelBlack = 9;

    epsilon = levelBlack;
    std::vector<double> scoresWhite;
    std::vector<double> scoresBlack;
    int whiteWin = 0, blackWin = 0;
    std::vector<double> history_levels = std::vector<double>{(double) levelBlack};


    for (unsigned int i = 0; i < partyNumber; ++i) {

        int countUp = 0;
        int countDown = 0;
        int countNothing = 0;
        int countTotal = 0;

        auto *engine = new openxum::core::games::kamisado::Engine(
                openxum::core::games::kamisado::SIMPLE,
                openxum::core::games::kamisado::Color::WHITE);

        auto
                *player_one = new openxum::ai::specific::kamisado::MCTSPlayer(
                openxum::core::games::kamisado::Color::WHITE, openxum::core::games::kamisado::Color::BLACK,
                engine, levelWhite * multiplicator);

        auto
                *player_two = new openxum::ai::specific::kamisado::MCTSPlayer(
                openxum::core::games::kamisado::Color::BLACK, openxum::core::games::kamisado::Color::WHITE,
                engine, epsilon * multiplicator);

        auto *current_player = player_one;
        std::vector<int> requestModify;

        while (not engine->is_finished()) {
            countTotal++;
            openxum::core::common::Move<openxum::core::games::kamisado::Decision>
                    move = current_player->get_move();


            engine->move(move);


            if (engine->current_color() == player_one->color()) {

                if (!engine->is_finished())
                    scoresWhite.push_back(findScore(current_player, levelWhite * multiplicator));
                current_player = player_one;


            } else {

                if (!engine->is_finished()) {

                    //Modification de niveau tous les bêta coups

                    double score = findScore(current_player, levelBlack * multiplicator);
                    double tmp = score;
                    if (scoresWhite.empty()) {
                        score = 0;
                    } else {
                        score = score - scoresWhite.back();
                    }

#if USE_LAMBDA == 0 and USE_AI == 0
                    int change = (levelBlack > levelWhite) ? 0 : ((levelBlack == levelWhite) ? 1 : 2 );
                    std::cout << i << "," << levelWhite << "," << levelBlack << "," << score << "," << change << "," << tmp << std::endl;
#endif

#if DEBUG == 1
                    std::cout << "DIFF: " << score << std::endl;
#endif

#if USE_LAMBDA == 1
                    if (score > lambda) { // Diminution du niveau
                        requestModify.push_back(-1);
                    } else if (score < -lambda) { // Augmentation du niveau
                        requestModify.push_back(1);
                    } else { // Ne fait rien
                        requestModify.push_back(0);
                    }

                    if (counter >= beta) {
                        counter = 1;

                        uint requestUp = std::count_if(requestModify.begin(), requestModify.end(),
                                                       [](const int &lvl) { return lvl == 1; });
                        uint requestDown = std::count_if(requestModify.begin(), requestModify.end(),
                                                         [](const int &lvl) { return lvl == -1; });
                        uint requestNothing = std::count_if(requestModify.begin(), requestModify.end(),
                                                            [](const int &lvl) { return lvl == 0; });

                        requestModify.clear();

                        if (requestUp > requestNothing && requestUp > requestDown) {//LEVEL Up
#if DEBUG == 1
                            std::cout << "up" << std::endl;
#endif
                            player_two->set_simulation_number((current_player->get_simulation_number() == 9) ? 9 : (
                                    current_player->get_simulation_number() + 1));
                            history_levels.push_back(player_two->get_simulation_number());
                            countUp++;
                        } else if (requestDown > requestNothing && requestDown > requestUp) { //LEVEL down
#if DEBUG == 1
                            std::cout << "down" << std::endl;
#endif
                            player_two->set_simulation_number((current_player->get_simulation_number() == 1) ? 1 : (
                                    current_player->get_simulation_number() - 1));
                            history_levels.push_back(player_two->get_simulation_number());
                            countDown++;
                        } else { // NOTHING
#if DEBUG == 1
                            std::cout << "nothing" << std::endl;
#endif
                            countNothing++;
                        }
                    } else {
                        counter++;
                    }
#endif

#if USE_AI == 1


                    std::vector<torch::jit::IValue> inputs;
                    std::vector<double> datas;


                    datas.push_back(score);
                    datas.push_back(tmp);

                    torch::Tensor tensor = torch::ones(datas.size());

                    for (uint u = 0; u < datas.size(); ++u) {
                        tensor[u] = datas[u];
                    }

                    inputs.push_back(tensor);

                    auto outputs = module.forward(inputs).toTensor();
                    auto preds = torch::max(outputs.slice(0, 0, 9), 0);
                    int ia_level = std::get<1>(preds).data().item().toInt() + 1;
#if DEBUG == 1
                    std::cout << ia_level << " " << levelWhite << " " << levelBlack << std::endl;
#endif
                    requestModify.push_back(ia_level);


                    if (counter >= beta) {
                        counter = 1;
                        int new_ia_level = std::round(
                                (double) std::accumulate(requestModify.begin(), requestModify.end(), 0.0) /
                                (double) requestModify.size());
                        requestModify.clear();


                        if (history_levels.back() < new_ia_level) {//LEVEL Up
#if DEBUG == 1
                            std::cout << "up" << std::endl;
#endif
#if USE_AI == 1
                            countUp += new_ia_level - history_levels.back();
#elif USE_LAMBDA == 1
                            countUp += 1;
#endif
                        } else if (history_levels.back() > new_ia_level) { //LEVEL down
#if DEBUG == 1
                            std::cout << "down" << std::endl;
#endif
#if USE_AI == 1
                            countDown += history_levels.back() - new_ia_level;
#elif USE_LAMBDA == 1
                            countDown+= 1;
#endif
                        } else { // NOTHING
#if DEBUG == 1
                            std::cout << "nothing" << std::endl;
#endif
                            countNothing++;
                        }

                        player_two->set_simulation_number(new_ia_level);
                        history_levels.push_back(new_ia_level);

                    } else
                        counter++;

#endif

                    scoresBlack.push_back(findScore(current_player, levelBlack));
                }
                current_player = player_two;
            }
        }


        stats.push_back(engine->winner_is());

        switch (engine->winner_is()) {
            case openxum::core::games::kamisado::Color::BLACK:
                blackWin++;
                break;
            case openxum::core::games::kamisado::Color::WHITE:
                whiteWin++;
                break;
        }

#if USE_LAMBDA == 1 or USE_AI == 1
        double newEpsilon =
                std::accumulate(history_levels.begin(), history_levels.end(), 0.0) / history_levels.size();

        std::cout << i << "," << levelWhite << "," << epsilon << "," << whiteWin
                  << "," << blackWin
                  << "," << countUp
                  << "," << countDown
                  << "," << countNothing
                  << "," << countTotal
                  << "," << newEpsilon
                  << std::endl;
#endif
        if (history_levels.size() > maxLevelInHistory)
            history_levels = std::vector<double>(history_levels.end() - maxLevelInHistory,
                                                 history_levels.end());
#if USE_LAMBDA == 1 or USE_AI == 1
        epsilon = std::round(newEpsilon);
#endif
        delete player_one;
        delete player_two;
        delete engine;

    }
//        }
//    }

    return EXIT_SUCCESS;
}