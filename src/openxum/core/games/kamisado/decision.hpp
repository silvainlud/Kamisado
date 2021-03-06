/**
 * @file openxum/core/games/kamisado/decision.hpp
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

#ifndef OPENXUM_CORE_GAMES_KAMISADO_DECISION_HPP
#define OPENXUM_CORE_GAMES_KAMISADO_DECISION_HPP

#include <openxum/core/common/decision.hpp>
#include <openxum/core/games/kamisado/coordinates.hpp>
#include <openxum/core/games/kamisado/decision_type.hpp>

namespace openxum {
namespace core {
namespace games {
namespace kamisado {

class Decision : public openxum::core::common::Decision
{
public:
  Decision() = default;

  Decision(const DecisionType &type, const Coordinates &to, const Coordinates &from);

  void decode(const std::string &) override;

  std::string encode() const override;

  const Coordinates &from() const
  { return _from; }

  void from_object(const nlohmann::json &) override;

  const Coordinates &to() const
  { return _to; }

  nlohmann::json to_object() const override;

  std::string to_string() const override
  {
    if (_type == MOVE) {
      return "move tower from " + _from.to_string() + " to " + _to.to_string();
    } else {
      return "pass";
    }
  }

  DecisionType type() const
  { return _type; }

private:
  DecisionType _type;
  Coordinates _from;
  Coordinates _to;
};

}
}
}
}

#endif