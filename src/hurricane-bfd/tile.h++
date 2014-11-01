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

#ifndef LIBHURRICANE_ELF__TILE_HXX
#define LIBHURRICANE_ELF__TILE_HXX

#include "tile_address.h++"
#include "instruction.h++"
#include <string>
#include <vector>

namespace hurricane_bfd {
    class tile {
    public:
        typedef std::shared_ptr<tile> ptr;

    private:
        const tile_address _addr;
        std::vector<instruction::ptr> _lo;
        std::vector<instruction::ptr> _hi;

    public:
        tile(const tile_address& addr,
             std::vector<instruction::ptr> lo,
             std::vector<instruction::ptr> hi);

        tile(const tile_address& addr,
             size_t lo_instruction_count,
             std::vector<instruction::ptr> instructions);

    public:
        const tile_address& address(void) const { return _addr; }
        const std::vector<instruction::ptr> lo(void) const { return _lo; }
        const std::vector<instruction::ptr> hi(void) const { return _hi; }
        
    public:
    };
}

#endif
