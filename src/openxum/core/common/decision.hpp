/**
 * @file openxum/core/common/decision.hpp
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

#ifndef OPENXUM_CORE_COMMON_DECISION_HPP
#define OPENXUM_CORE_COMMON_DECISION_HPP

#include <nlohmann/json.hpp>
#include <vector>

namespace openxum {
namespace core {
namespace common {

class Decision
{
public:
  Decision() = default;

  virtual ~Decision() = default;

  virtual void decode(const std::string &) = 0;

  virtual std::string encode() const = 0;

  virtual void from_object(const nlohmann::json &) = 0;

  virtual nlohmann::json to_object() const = 0;

  virtual std::string to_string() const = 0;
};

}
}
}

#endif