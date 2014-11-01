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

#ifndef LIBHURRICANE_ELF__TILE_ADDRESS_HXX
#define LIBHURRICANE_ELF__TILE_ADDRESS_HXX

#include <string>

namespace hurricane_bfd {
    class tile_address {
    private:
        size_t _x;
        size_t _y;

    public:
        tile_address(size_t x, size_t y);

    public:
        std::string name(void) const;
        std::string mangled_name(void) const;
        void update_max(size_t& x, size_t& y) const;
    };
}

#endif
