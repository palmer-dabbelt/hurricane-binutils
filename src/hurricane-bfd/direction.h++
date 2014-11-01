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

#ifndef LIBHURRICANE_ELF__DIRECTION_HXX
#define LIBHURRICANE_ELF__DIRECTION_HXX

#include <vector>

namespace hurricane_bfd {
    /* Represents the different network directions that Hurricane
     * operations can target. */
    enum class direction {
        /* FIXME: While these constants have a direct mapping to the
         * DREAMER implementation, that's going to change because we
         * need things to be paramaterizable. */
        W = 0,
        N = 1,
        E = 2,
        S = 3
    };

    static std::vector<enum direction> all_directions =
    {
        direction::W,
        direction::N,
        direction::E,
        direction::S
    };
}

#endif
