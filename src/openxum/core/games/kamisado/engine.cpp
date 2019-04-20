/**
 * @file openxum/core/games/kamisado/engine.cpp
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

#include <openxum/core/games/kamisado/engine.hpp>

#include <iostream>

namespace openxum {
    namespace core {
        namespace games {
            namespace kamisado {

                static int initial_colors[8][8] = {
                        {ORANGE, BLUE,   PURPLE, PINK,   YELLOW, RED,    GREEN,  BROWN},
                        {RED,    ORANGE, PINK,   GREEN,  BLUE,   YELLOW, BROWN,  PURPLE},
                        {GREEN,  PINK,   ORANGE, RED,    PURPLE, BROWN,  YELLOW, BLUE},
                        {PINK,   PURPLE, BLUE,   ORANGE, BROWN,  GREEN,  RED,    YELLOW},
                        {YELLOW, RED,    GREEN,  BROWN,  ORANGE, BLUE,   PURPLE, PINK},
                        {BLUE,   YELLOW, BROWN,  PURPLE, RED,    ORANGE, PINK,   GREEN},
                        {PURPLE, BROWN,  YELLOW, BLUE,   GREEN,  PINK,   ORANGE, RED},
                        {BROWN,  GREEN,  RED,    YELLOW, PINK,   PURPLE, BLUE,   ORANGE}
                };

                std::string Engine::GAME_NAME = "kamisado";

                // constructors
                Engine::Engine(int type, int color)
                        :_color(color), _type(type)
                {
                    for (int i = 0; i < 8; i++) {
                        _black_towers.emplace_back(State(i, 0, initial_colors[i][0]));
                    }
                    for (int i = 0; i < 8; i++) {
                        _white_towers.emplace_back(State(i, 7, initial_colors[i][7]));
                    }
                    _phase = MOVE_TOWER;
                    _play_color = NONE;
                }

                // public methods
                void Engine::apply_moves(const openxum::core::common::Moves& moves)
                {
                    for (auto m: moves) {
                        move(m);
                    }
                }

                Engine* Engine::clone() const
                {
                    auto e = new Engine(_type, _color);

                    for (const State& s: _black_towers) {
                        e->_black_towers.push_back(s);
                    }
                    for (const State& s: _white_towers) {
                        e->_white_towers.push_back(s);
                    }
                    e->_phase = _phase;
                    e->_play_color = _play_color;
                    return e;
                }

                int Engine::current_color() const
                {
                    return _color;
                }

                openxum::core::common::Moves Engine::get_possible_move_list() const
                {
                    openxum::core::common::Moves moves;
                    const Coordinates& playable_tower = find_playable_tower(_color);

                    if (playable_tower.is_valid()) {
                        const std::vector<Coordinates>& list = get_possible_moving_list(
                                State(playable_tower.x(), playable_tower.y(), _color));

                        for (const Coordinates& c: list) {
                            moves.push_back(new Move(playable_tower, c));
                        }
                    } else {
                        const std::vector<State>& towers = get_towers(_color);

                        for (const State& s: towers) {
                            const std::vector<Coordinates>& list = get_possible_moving_list(
                                    State(s.x(), s.y(), _color));

                            for (const Coordinates& c: list) {
                                moves.push_back(new Move(s.coordinates(), c));
                            }
                        }
                    }
                    return moves;
                }

                bool Engine::is_finished() const
                {
                    return _phase == FINISH;
                }

                void Engine::move(const openxum::core::common::Move* move)
                {
                    auto* m = dynamic_cast<const openxum::core::games::kamisado::Move*>(move);

                    move_tower(m->from(), m->to());
                }

                int Engine::winner_is() const
                {
                    if (is_finished()) {
                        return _color;
                    } else {
                        return NONE;
                    }
                }

                // private methods
                void Engine::change_color()
                {
                    _color = next_color(_color);
                }

                Coordinates Engine::find_playable_tower(int color) const
                {
                    Coordinates result;

                    if (_play_color != NONE) {
                        std::vector<State> towers = get_towers(color);

                        for (const State& s : towers) {
                            if (s.color() == _play_color) {
                                result = s.coordinates();
                            }
                        }
                    }
                    return result;
                }

                State Engine::find_towers2(const Coordinates& coord, int color) const
                {
                    State result;
                    std::vector<State> towers = get_towers(color);
                    bool found = false;
                    int i = 0;

                    while (i < 8 && !found) {
                        if (towers[i].x() == coord.x() && towers[i].y() == coord.y()) {
                            result = towers[i];
                            found = true;
                        } else i++;
                    }
                    return result;
                }

                std::vector<Coordinates> Engine::get_possible_moving_list(const State& tower) const
                {
                    std::vector<Coordinates> moves;
                    int step = tower.color() == BLACK ? 1 : -1;
                    int limit = tower.color() == BLACK ? 8 : -1;

                    //column
                    {
                        int line = tower.y() + step;
                        Coordinates coordinates(tower.x(), line);

                        while (line != limit and is_empty(coordinates)) {
                            moves.push_back(coordinates);
                            line += step;
                            coordinates = Coordinates(tower.x(), line);
                        }
                    }

                    // right diagonal
                    {
                        int column = tower.x() + 1;
                        int line = tower.y() + step;
                        Coordinates coordinates(column, line);

                        while (line != limit && column != 8 and is_empty(coordinates)) {
                            moves.push_back(coordinates);
                            line += step;
                            ++column;
                            coordinates = Coordinates(column, line);
                        }
                    }

                    // left diagonal
                    {
                        int column = tower.x() - 1;
                        int line = tower.y() + step;
                        Coordinates coordinates(column, line);

                        while (line != limit && column != -1 and is_empty(coordinates)) {
                            moves.push_back(coordinates);
                            line += step;
                            --column;
                            coordinates = Coordinates(column, line);
                        }
                    }
                    return moves;
                }

                const std::vector<State>& Engine::get_towers(int color) const
                {
                    return color == BLACK ? _black_towers : _white_towers;
                }

                bool Engine::is_empty(const Coordinates& coordinates) const
                {
                    bool found = false;
                    int i = 0;

                    while (i < 8 and not found) {
                        if (_black_towers[i].x() == coordinates.x() and _black_towers[i].y() == coordinates.y()) {
                            found = true;
                        } else {
                            i++;
                        }
                    }
                    i = 0;
                    while (i < 8 and not found) {
                        if (_white_towers[i].x() == coordinates.x() and _white_towers[i].y() == coordinates.y()) {
                            found = true;
                        } else {
                            i++;
                        }
                    }
                    return not found;
                }

                void Engine::move_tower(const Coordinates& selected_tower, const Coordinates& selected_cell)
                {
                    State tower = find_towers2(selected_tower, _color);

                    if (tower.x() != -1) {
                        tower = selected_cell;
                    }
                    if ((_color == BLACK and tower.y() == 7) or (_color == WHITE and tower.y() == 0)) {
                        _phase = FINISH;
                    } else {
                        _play_color = initial_colors[tower.x()][tower.y()];
                        if (not pass(next_color(_color))) {
                            change_color();
                        } else {
                            const Coordinates& playable_tower = find_playable_tower(next_color(_color));

                            _play_color = initial_colors[playable_tower.x()][playable_tower.y()];
                            if (pass(_color)) {
                                _phase = FINISH;
                                change_color();
                            }
                        }
                    }
                }

                int Engine::next_color(int color)
                {
                    return color == WHITE ? BLACK : WHITE;
                }

                bool Engine::pass(int color) const
                {
                    const Coordinates& playable_tower = find_playable_tower(color);
                    const std::vector<Coordinates>& list = get_possible_moving_list(
                            State(playable_tower.x(), playable_tower.y(), _color));

                    return list.empty();
                }
            }
        }
    }
}