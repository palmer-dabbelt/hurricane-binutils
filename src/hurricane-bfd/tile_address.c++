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

#include "tile_address.h++"
#include <sstream>
using namespace hurricane_bfd;

tile_address::tile_address(size_t x, size_t y)
    : _x(x),
      _y(y)
{
}

std::string tile_address::name_with_space(void) const
{
    std::stringstream ss;
    ss << "(" << _x << ", " << _y << ")";
    return ss.str();
}

std::string tile_address::name_without_space(void) const
{
    std::stringstream ss;
    ss << "(" << _x << "," << _y << ")";
    return ss.str();
}

std::string tile_address::mangled_name(void) const
{
    std::stringstream ss;
    ss << _x << "__" << _y;
    return ss.str();
}

void tile_address::update_max(size_t& x, size_t& y) const
{
    if (_x > x)
        x = _x;
    if (_y > y)
        y = _y;
}
