/**
 * @file openxum/core/games/kikotsoka/decision.cpp
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

#include <openxum/core/games/kikotsoka/decision.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kikotsoka {

Decision::Decision(const DecisionType::Values &type,
                   const Color &color,
                   const Coordinates &to,
                   int index)
    : _type(type), _color(color), _to(to), _index(index)
{}

void Decision::decode(const std::string &str)
{
  _color = str[0] == 'B' ? Color::BLACK : Color::WHITE;
  if (str[1] == '-') {
    _type = DecisionType::PASS;
    _to = Coordinates();
    _index = -1;
  } else if (str[1] == 'P') {
    _type = DecisionType::CHOICE_PATTERN;
    _to = Coordinates();
    _index = str[1] - '0';
  } else if (str[1] == 'p') {
    _type = DecisionType::CHOICE_PIECE;
    _to = Coordinates();
    _index = str[1] - '0';
  } else {
    _type = str[1] == '+' ? DecisionType::PUT_PIECE : str[1] == '*' ? DecisionType::PUT_SHIDO
                                                                    : DecisionType::PUT_INITIAL_SHIDO;
    if (str.size() == 4) {
      _to = Coordinates(str[2] - 'A', str[3] - '1');
    } else {
      _to = Coordinates(str[2] - 'A', (str[3] - '0') * 10 + (str[4] - '0') - 1);
    }
    _index = -1;
  }
}

std::string Decision::encode() const
{
  if (_type == DecisionType::PASS) {
    return std::string(_color == Color::BLACK ? "B" : "W") + "-";
  } else if (_type == DecisionType::CHOICE_PATTERN) {
    return std::string(_color == Color::BLACK ? "B" : "W") + "P" + std::to_string(_index);
  } else if (_type == DecisionType::CHOICE_PIECE) {
    return std::string(_color == Color::BLACK ? "B" : "W") + "p" + std::to_string(_index);
  } else if (_type == DecisionType::PUT_PIECE) {
    return std::string(_color == Color::BLACK ? "B" : "W") + "+" + _to.to_string();
  } else if (_type == DecisionType::PUT_SHIDO) {
    return std::string(_color == Color::BLACK ? "B" : "W") + "*" + _to.to_string();
  } else if (_type == DecisionType::PUT_INITIAL_SHIDO) {
    return std::string(_color == Color::BLACK ? "B" : "W") + "#" + _to.to_string();
  }
  return "";
}

void Decision::from_object(const nlohmann::json &json)
{
  Coordinates to;

  to.from_object(json["to"]);
  _type = DecisionType::Values(json["type"].get<int>());
  _color = Color(json["color"].get<int>());
  _to = to;
  _index = json["index"].get<int>();
}

nlohmann::json Decision::to_object() const
{
  nlohmann::json json;

  json["type"] = _type;
  json["color"] = _color;
  json["to"] = _to.to_object();
  json["index"] = _index;
  return json;
}

std::string Decision::to_string() const
{
  if (_type == DecisionType::PASS) {
    return "PASS";
  } else if (_type == DecisionType::CHOICE_PATTERN) {
    return "CHOICE pattern " + std::to_string(_index);
  } else if (_type == DecisionType::CHOICE_PIECE) {
    if (_index == 0) {
      return "CHOICE shido";
    } else {
      return "CHOICE piece";
    }
  } else if (_type == DecisionType::PUT_PIECE) {
    return "PUT " + std::string(_color == Color::BLACK ? "black" : "white") + " PIECE at " +
        _to.to_string();
  } else if (_type == DecisionType::PUT_SHIDO) {
    return "PUT " + std::string(_color == Color::BLACK ? "black" : "white") + " SHIDO at " +
        _to.to_string();
  } else if (_type == DecisionType::PUT_INITIAL_SHIDO) {
    return "PUT INITIAL " + std::string(_color == Color::BLACK ? "black" : "white") + " SHIDO at " +
        _to.to_string();
  }
  return "";
}

}
}
}
}