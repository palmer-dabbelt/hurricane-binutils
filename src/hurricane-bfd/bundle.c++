/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of hurricane-binutils.
 * 
 * hurricane-binutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * hurricane-binutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with hurricane-binutils.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bundle.h++"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <functional>
#include <locale>
#include <sstream>
#include <map>
#include <memory>
#include <cstdint>
#include <cstdlib>
using namespace hurricane_bfd;

bundle::bundle(const std::vector<instruction::ptr>& instructions)
    : _instructions(instructions),
      _has_debug(false)
{
}

bundle::bundle(const std::vector<instruction::ptr>& instructions,
               std::string debug)
    : _instructions(instructions),
      _has_debug(true),
      _debug(debug)
{
}

std::string bundle::jrb_string(void) const
{
    std::stringstream ss;
    bool first = true;

    for (const auto& inst: instructions()) {
        if (first == false)
            ss << "; ";

        ss << inst->jrb_string();

        first = false;
    }

    return ss.str();
}

std::string bundle::as_string(void) const
{
    std::stringstream ss;
    bool first = true;

    ss << "{ ";

    for (const auto& inst: instructions()) {
        if (first == false)
            ss << " ; ";

        ss << inst->as_string();

        first = false;
    }

    ss << " }";

    return ss.str();
}

bundle::ptr bundle::parse_hex(const std::string hex)
{
    std::vector<instruction::ptr> is;

    auto add = [&](const instruction::ptr& p)
        {
            if (p != NULL)
                is.push_back(p);
        };

    add(instruction::parse_hex_alu(hex, all_directions[0], true));
    for (const auto dir: all_directions)
        add(instruction::parse_hex_alu(hex, dir, false));
    for (const auto dir: all_directions)
        add(instruction::parse_hex_net(hex, dir));

    // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    auto rtrim = [](const std::string &s_in)
        {
            std::string s = s_in;
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
        };

    if (strstr(hex.c_str(), " ") == NULL)
        return std::make_shared<bundle>(is);
    else {
        std::string debug = rtrim(strstr(hex.c_str(), " ") + 1);
        return std::make_shared<bundle>(is, debug);
    }
}
