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

#include "tile.h++"
using namespace hurricane_bfd;

static std::vector<instruction::ptr>
lo(const std::vector<instruction::ptr>& instructions,
   size_t count);

static std::vector<instruction::ptr>
hi(const std::vector<instruction::ptr>& instructions,
   size_t count);

tile::tile(const tile_address& addr,
           size_t lo_instruction_count,
           std::vector<instruction::ptr> instructions)
    : _addr(addr),
      _lo(::lo(instructions, lo_instruction_count)),
      _hi(::hi(instructions, lo_instruction_count))
{
}


std::vector<instruction::ptr>
lo(const std::vector<instruction::ptr>& instructions,
   size_t count)
{
    std::vector<instruction::ptr> out(count);

    if (count > instructions.size()) {
        fprintf(stderr, "lo_count greater than instruction count: %llu %llu\n",
                (long long unsigned)count,
                (long long unsigned)instructions.size()
            );
        abort();
    }

    for (size_t i = 0; i < count; ++i)
        out[i] = instructions[i];

    return out;
}

std::vector<instruction::ptr>
hi(const std::vector<instruction::ptr>& instructions,
   size_t count)
{
    std::vector<instruction::ptr> out(instructions.size() - count);

    for (size_t i = count; i < instructions.size(); ++i)
        out[i - count] = instructions[i];

    return out;
}
