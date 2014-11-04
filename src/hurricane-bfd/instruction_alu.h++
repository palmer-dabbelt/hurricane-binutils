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

#ifndef LIBHURRICANE_BFD__INSTRUCTION_ALU_HXX
#define LIBHURRICANE_BFD__INSTRUCTION_ALU_HXX

#include "instruction.h++"

namespace hurricane_bfd {
    /* Represents a network instruction -- note that you probably
     * don't want to depend on this outside of the BFD library, as it
     * should go away eventually...  As such there are no comments
     * here, it's just a bit of duplicated functionality from
     * instruction and some virtual overrides. */
    class instruction_alu: public instruction {
        friend class instruction;

    public:
        instruction_alu(const inst_t& bits);
        instruction_alu(const inst_t& bits, const std::string debug);

    protected:
        bool sanity_check(void) const;

    public:
        virtual std::string jrb_string(void) const;
        virtual std::string as_string(void) const;

    public:
        static instruction::ptr parse_hex(const std::string hex);
    };
}

#endif
