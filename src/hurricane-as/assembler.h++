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

#ifndef HURRICANE_AS__ASSEMBLER_HXX
#define HURRICANE_AS__ASSEMBLER_HXX

#include <hurricane-bfd/object.h++>
#include <string>

namespace hurricane_as {
    /* This is pretty much only part of a different library than
     * hurricane_bfd because that's how GNU does it. */
    namespace assembler {
        /* Reads an assembly file and produces an in-memory
         * representation of the object file that can then be
         * printed. */
        hurricane_bfd::object::ptr read_as_file(const std::string& filename);
        hurricane_bfd::object::ptr read_as_file(FILE* file);
    }
}

#endif
