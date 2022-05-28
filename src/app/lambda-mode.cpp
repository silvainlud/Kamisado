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

///Permet d'afficher les sortie standards dans le but de débuger le program (0=FALSE, 1=TRUE)
#define DEBUG 0

using json = nlohmann::json;

///Nombre de coup avant changement de niveaux
int beta = 5;
///Interval à respecter [-lambda;lambda]
double lambda = 0.05;
///Niveau de départ de l'IA adaptative
int epsilon = 5;
int counter = 0;


double findScore(openxum::ai::specific::kamisado::MCTSPlayer *player, unsigned int level) {
    player->init_search();
    for (unsigned int i = 0; i < level; ++i) {
        player->simulate_one_game_from_root();
    }
    return player->get_final_choice()->compute_score();
}

int main(int argc, const char **argv) {


    if (argc < 4) {
        std::cerr << argv[0] << " <startLevel> <targetLevel> <nombre de partie>" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<int> stats;

    const unsigned int partyNumber = std::stoi(argv[3]);

    std::cout << "id" << "," << "levelWhite" << "," << "levelBlack" << ","
              << "whiteWin"
              << "," << "blackWin"
              << "," << "countUp"
              << "," << "countDown"
              << "," << "countNothing"
              << "," << "countTotal"
              << "," << "epsilon"
              << std::endl;

    const uint levelWhite = std::stoi(argv[2]);
    const uint levelBlack = std::stoi(argv[1]);

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
                engine, levelWhite);

        auto
                *player_two = new openxum::ai::specific::kamisado::MCTSPlayer(
                openxum::core::games::kamisado::Color::BLACK, openxum::core::games::kamisado::Color::WHITE,
                engine, epsilon);

        auto *current_player = player_one;
        std::vector<int> requestModify;

        while (not engine->is_finished()) {
            countTotal++;
            openxum::core::common::Move<openxum::core::games::kamisado::Decision>
                    move = current_player->get_move();


            engine->move(move);


            if (engine->current_color() == player_one->color()) {

                if (!engine->is_finished())
                    scoresWhite.push_back(findScore(current_player, levelWhite));
                current_player = player_one;


            } else {

                if (!engine->is_finished()) {

                    //Modification de niveau tous les bêta coups

                    double score = findScore(current_player, levelBlack);
                    double tmp = score;
                    if (scoresWhite.empty()) {
                        score = 0;
                    } else {
                        score = score - scoresWhite.back();
                    }

#if DEBUG == 1
                    std::cout << "DIFF: " << score << std::endl;
#endif

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


        epsilon = std::round(newEpsilon);
        delete player_one;
        delete player_two;
        delete engine;

    }

    return EXIT_SUCCESS;
}