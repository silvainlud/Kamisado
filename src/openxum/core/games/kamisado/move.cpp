/**
 * @file openxum/core/games/kamisado/move.cpp
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

#include <openxum/core/games/kamisado/move.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kamisado {

Move::Move(const MoveType &type, const Coordinates &from, const Coordinates &to)
    : _type(type), _from(from), _to(to)
{}

openxum::core::common::Move *Move::clone() const
{
  return new Move(_type, _from, _to);
}

void Move::decode(const std::string &str)
{
  if (str[0] == 'M') {
    _type = MOVE;
  } else {
    _type = PASS;
  }
  _from = Coordinates(str[1] - 'a', str[2] - '1');
  _to = Coordinates(str[3] - 'a', str[4] - '1');
}

std::string Move::encode() const
{
  return (_type == MOVE ? "M" : "P") + _from.to_string() + _to.to_string();
}

void Move::from_object(const nlohmann::json &json)
{
  Coordinates from, to;

  from.from_object(json["from"]);
  to.from_object(json["to"]);
  _type = MoveType(json["type"].get<int>());
  _from = from;
  _to = to;
}

nlohmann::json Move::to_object() const
{
  nlohmann::json json;

  json["type"] = _type;
  json["from"] = _from.to_object();
  json["to"] = _to.to_object();
  return json;
}

}
}
}
}