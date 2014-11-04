/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of dreamer-binutils.
 * 
 * dreamer-binutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * dreamer-binutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with dreamer-binutils.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "label.h++"
using namespace hurricane_as;

label::offset::ptr label::offset::without_relative(void) const
{
    return std::make_shared<offset>(_absolute, 0);
}

label::offset::ptr label::offset::increment(void) const
{
    return std::make_shared<offset>(_absolute + 1, _relative + 1);
}

label::offset::offset(void)
    : _absolute(0),
      _relative(0)
{
}

label::offset::offset(uint64_t absolute, uint64_t relative)
    : _absolute(absolute),
      _relative(relative)
{
}

label::label(std::string name)
    : _name(name),
      _base(std::make_shared<label::offset>())
{
}

label::label(std::string name, offset::ptr base)
    : _name(name),
      _base(base->without_relative())
{
}
