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

#ifndef LIBHURRICANE_ELF__INSTRUCTION_HXX
#define LIBHURRICANE_ELF__INSTRUCTION_HXX

#include <memory>
#include <cstdint>

namespace hurricane_bfd {
    /* Stores a single instruction. */
    class instruction {
    public:
        typedef std::shared_ptr<instruction> ptr;

    private:
        union inst {
            uint32_t bits;

            /* These bits are true for all instruction encoding
             * formats, so you can always look at them!  This matches
             * the definiton of the FixInst from Scala. */
            struct {
                unsigned int out : 4;
                unsigned int in  : 2;
                unsigned int zi  : 5;
                unsigned int yi  : 5;
                unsigned int iy  : 1;
                unsigned int xi  : 5;
                unsigned int di  : 5;
                unsigned int op  : 5;
            } inst __attribute__((packed));

            /* The format used for "lit". */
            struct {
                unsigned int out : 4;
                unsigned int in  : 2;
                unsigned int lit : 16;
                unsigned int di  : 5;
                unsigned int op  : 5;
            } lit __attribute__((packed));
        };
        typedef union inst inst_t;

    private:
        const inst_t _bits;
        const bool _has_debug;
        const std::string _debug;

    public:
        /* Allows for the construction of an instruction by
         * passing in the raw bits that represent it -- note that
         * you don't want to call these directly, instead call a
         * more descriptive static method below.  They're only
         * public because of std::shared_ptr.*/
        instruction(const inst_t& bits);
        instruction(const inst_t& bits, const std::string debug);

    private:
        /* Runs some sanity checks on this instruction -- this
         * will happen every time an instruction is generated, but
         * it'll happen as part of one of the static functions
         * because they can return NULL. */
        bool sanity_check(void) const;

    public:
        /* Accessor functions. */
        inst_t bits(void) const { return _bits; }
        bool has_debug(void) const { return _has_debug; }
        std::string debug(void) const {
            if (_has_debug == false)
                abort();
            return _debug;
        }

        /* Returns a string representation of this instruction. */
        std::string to_string(void) const;

        /* Returns a new copy of this instruction with some debug
         * info attached to it. */
        ptr with_debug(const std::string debug);

    public:
        /* Parses an instruction from a HEX string as it would
         * appear in a HEX file. */
        static ptr parse_hex(const std::string hex);
    };
}

#endif
