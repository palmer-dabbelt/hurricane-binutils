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

#ifndef LIBHURRICANE_ELF__OPCODE_HXX
#define LIBHURRICANE_ELF__OPCODE_HXX

#include <string>
#include <vector>

namespace hurricane_bfd {
    /* Represents the possible opcodes a Hurricane instruction can
     * have.  Note that you shouldn't depend on the exact value of
     * these opcodes but should instead compare/switch on them, as the
     * bit value will depend on an encoding of the Hurricane opcode
     * space which will be different for different parameter sets. */
    enum class opcode {
        /* FIXME: While these opcodes currently have a direct mapping
         * to the DREAMER opcodes, this isn't actually correct -- we
         * need a little layer in between to make this code can all be
         * paramaterized correctly. */
        NO = 0,
        RST = 1,
        LIT = 2,
        RND = 4,
        EAT = 5,
        NOT = 6,
        AND = 7,
        OR = 8,
        XOR = 9,
        EQ = 10,
        NEQ = 11,
        MUX = 12,
        LSH = 13,
        RSH = 14,
        ARSH = 15,
        MSK = 16,
        CAT = 17,
        ADD = 18,
        SUB = 19,
        LT = 20,
        GTE = 21,
        MUL = 22,
        LOG2 = 23,
        LD = 24,
        ST = 25,
        LDI = 26,
        STI = 27
    };

    static std::vector<enum opcode> all_opcodes =
    {
        opcode::NO,
        opcode::RST,
        opcode::LIT,
        opcode::RND,
        opcode::EAT,
        opcode::NOT,
        opcode::AND,
        opcode::OR,
        opcode::XOR,
        opcode::EQ,
        opcode::NEQ,
        opcode::MUX,
        opcode::LSH,
        opcode::RSH,
        opcode::ARSH,
        opcode::MSK,
        opcode::CAT,
        opcode::ADD,
        opcode::SUB,
        opcode::LT,
        opcode::GTE,
        opcode::MUL,
        opcode::LOG2,
        opcode::LD,
        opcode::ST,
        opcode::LDI,
        opcode::STI
    };
}

namespace std {
    string to_string(const hurricane_bfd::opcode& op);
}

#endif
