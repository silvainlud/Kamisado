/**
 * @file openxum/core/games/kamisado/engine.hpp
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

#ifndef OPENXUM_CORE_GAMES_KAMISADO_ENGINE_HPP
#define OPENXUM_CORE_GAMES_KAMISADO_ENGINE_HPP

#include <openxum/core/common/engine.hpp>

#include <openxum/core/games/kamisado/color.hpp>
#include <openxum/core/games/kamisado/coordinates.hpp>
#include <openxum/core/games/kamisado/move.hpp>
#include <openxum/core/games/kamisado/phase.hpp>
#include <openxum/core/games/kamisado/state.hpp>
#include <openxum/core/games/kamisado/tower_color.hpp>

#include <vector>

namespace openxum {
    namespace core {
        namespace games {
            namespace kamisado {

                class Engine : public openxum::core::common::Engine {
                public:
                    Engine() = default;

                    Engine(int type, int color);

                    ~Engine() override;

                    openxum::core::common::Move* build_move() const override
                    { return new Move(); }

                    Engine* clone() const override;

                    int current_color() const override;

                    Coordinates find_playable_tower(int) const;

                    const std::string& get_name() const override { return GAME_NAME; }

                    openxum::core::common::Moves get_possible_move_list() const override;

                    std::string id() const override;

                    bool is_finished() const override;

                    void move(const openxum::core::common::Move* move) override;

                    void parse(const std::string&) override { }

                    int type() const { return _type; }

                    std::string to_string() const override;

                    int winner_is() const override;

                private:
                    void change_color();

                    State& find_towers2(const Coordinates&, int);

                    std::vector<Coordinates> get_possible_moving_list(const State& tower, int color) const;

                    bool is_empty(const Coordinates&) const;

                    void move_tower(const Coordinates& selected_tower, const Coordinates& selected_cell);

                    int next_color(int color);

                    bool pass(int) const;

                    int _color;
                    int _type;
                    std::vector<State> _black_towers;
                    std::vector<State> _white_towers;
                    TowerColor::Values _play_color;
                    int _phase;

                    static std::string GAME_NAME;
                };

            }
        }
    }
}

#endif //KAMISADO_ENGINE_H
