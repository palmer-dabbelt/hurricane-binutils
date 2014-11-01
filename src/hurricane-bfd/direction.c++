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

#include "direction.h++"
#include <sstream>
using namespace hurricane_bfd;

std::string std::to_string(const enum hurricane_bfd::direction& dir)
{
    switch (dir) {
    case direction::W:
        return "W";
    case direction::N:
        return "N";
    case direction::E:
        return "E";
    case direction::S:
        return "S";
    }

    fprintf(stderr, "std::to_string(hurricane_bfd::direction) overran switch\n");
    fprintf(stderr, "  This is frequently a compile corruption problem\n");
    abort();
}

std::string std::to_string(const std::vector<enum hurricane_bfd::direction>& dirs)
{
    std::stringstream ss;

    for (const auto& dir: dirs)
        ss << std::to_string(dir);

    return ss.str();
}

std::string std::to_string(const std::map<enum hurricane_bfd::direction, bool>& dirs)
{
    std::stringstream ss;

    for (const auto& dir: all_directions) {
        auto l = dirs.find(dir);
        if (l == dirs.end())
            continue;
        if (l->second == false)
            continue;
        ss << std::to_string(dir);
    }

    return ss.str();
}
