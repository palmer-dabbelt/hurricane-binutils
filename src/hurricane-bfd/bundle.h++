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

#ifndef HURRICANE_BFD__BUNDLE_HXX
#define HURRICANE_BFD__BUNDLE_HXX

#include "instruction.h++"
#include <vector>

namespace hurricane_bfd {
    /* Represents a single bundle of instructions, like a regular VLIW
     * machine.  A bundle is all commited at the same time and
     * therefor may be executed in parallel. */
    class bundle {
    public:
        typedef std::shared_ptr<bundle> ptr;

    private:
        const std::vector<instruction::ptr> _instructions;

    public:
        bundle(const instruction::ptr& a);
        bundle(const instruction::ptr& a, const instruction::ptr& b);
        bundle(const std::vector<instruction::ptr>& instructions);

    public:
        const std::vector<instruction::ptr> instructions(void) const
            { return _instructions; }

    public:
        /* Returns a string representation of this instruction, in a
         * few different formats:
         * - "jrb_string" matches Jonathan's disassembler
         * - "as_string" looks like regular assembly
         */
        std::string jrb_string(void) const;
        std::string as_string(void) const;

    public:
        /* Parses an instruction from a HEX string as it would appear
         * in a HEX file -- this lives here because it has to touch
         * the binary representation of a bundle. */
        static ptr parse_hex(const std::string hex);
    };
}

#endif
