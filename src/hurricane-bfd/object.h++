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

#ifndef LIBHURRICANE_ELF__OBJECT_HXX
#define LIBHURRICANE_ELF__OBJECT_HXX

#include "tile.h++"
#include <memory>
#include <string>
#include <vector>

namespace hurricane_bfd {
    /* Represents the contents of a single object file, along with
     * mechanisms to serialize and deserialize these files into
     * different file formats. */
    class object {
    public:
        typedef std::shared_ptr<object> ptr;

    private:
        std::vector<tile::ptr> _tiles;

    public:
        /* Creates an empty object file.  This is useful if you're
         * going to do something like writing an assembler. */
        object(void);

        /* Creates a new in-memory object representation, given
         * every tile in the system that has code. */
        object(const std::vector<tile::ptr>& tiles);

    public:
        const std::vector<tile::ptr>& tiles(void) const { return _tiles; }

    public:
        /* Writes this object out to a HEX file. */
        void write_hex_file(const std::string& filename);
        void write_hex_file(FILE *file);

    public:
        /* Attempts to determine the type of this file by parsing it
         * as every file type.  This is probably the parser you should
         * be using, unless for some reason you're trying to parse
         * only a particular file format. */
        static ptr read_file(const std::string& filename);

        /* Creates a new object by reading a HEX file that
         * represents a single program. */
        static ptr read_hex_file(const std::string& filename);
        static ptr read_hex_file(FILE *file);
    };
}

#endif
