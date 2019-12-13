/**
 * @file openxum/core/common/move.hpp
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

#ifndef OPENXUM_CORE_COMMON_MOVE_HPP
#define OPENXUM_CORE_COMMON_MOVE_HPP

#include <openxum/core/common/decision.hpp>
#include <vector>

namespace openxum {
namespace core {
namespace common {

template<class Decision>
class Move : public std::vector<Decision>
{
  typedef typename std::vector<Decision> type;

public:
  Move() = default;

  Move(const Decision &decision)
  { type::push_back(decision); }

  Move(std::initializer_list<Decision> list) : std::vector<Decision>(list)
  {}

  virtual ~Move() = default;

  Move *clone() const
  { return new Move(*this); }

  void decode(const std::string &str)
  {
    if (str[0] == '<') {
      std::string::size_type start = 2;
      size_t pos = str.find_first_of(' ', start);

      while (pos != std::string::npos) {
        if (pos != start) {
          Decision decision;

          decision.decode(str.substr(start, pos - start));
          type::push_back(decision);
        }
        start = pos + 1;
        pos = str.find_first_of(' ', start);
      }
    } else {
      Decision decision;

      decision.decode(str);
      type::push_back(decision);
    }
  }

  std::string encode() const
  {
    std::string str;

    if (type::size() > 1) {
      std::for_each(type::begin(), type::end(),
                    [&str](const Decision &e) {
                      if (str.empty()) {
                        str = e.encode();
                      } else {
                        str += ";" + e.encode();
                      }
                    });
      return "[" + str + "]";
    } else {
      return type::front().encode();
    }
  }

  void from_object(const nlohmann::json &json)
  {
    if (json.is_array()) {
      std::for_each(json.begin(), json.end(),
                    [this](const nlohmann::json &e) {
                      Decision decision;

                      decision.from_object(e);
                      type::push_back(decision);
                    });
    } else {
      Decision decision;

      decision.from_object(json);
      type::push_back(decision);
    }
  }

  nlohmann::json to_object() const
  {
    if (type::size() > 1) {
      nlohmann::json json = nlohmann::json::array();

      std::for_each(type::begin(), type::end(),
                    [&json](const Decision &e) { json.push_back(e.to_object()); });
      return json;
    } else {
      return type::front().to_object();
    }
  }

  std::string to_string() const
  {
    std::string str;

    if (type::size() > 1) {
      std::for_each(type::begin(), type::end(), [&str](const Decision &e) {
        if (str.empty()) {
          str = e.to_string();
        } else {
          str += " ; " + e.to_string();
        }
      });
      return "(" + str + ")";
    } else {
      return type::back().to_string();
    }
  }
};

template<class Decision>
class Moves : public std::vector<Move<Decision>>
{
public:
  Moves() = default;

  ~Moves() = default;
};

}
}
}

#endif