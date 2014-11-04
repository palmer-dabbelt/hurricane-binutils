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

#include "opcode.h++"
#include <cstdlib>
#include <cstring>
using namespace hurricane_bfd;

std::string std::to_string(const hurricane_bfd::opcode& op)
{
    switch (op) {
    case opcode::NO:
        return "nop";
    case opcode::RST:
        return "rst";
    case opcode::LIT:
        return "lit";
    case opcode::RND:
        return "rnd";
    case opcode::EAT:
        return "eat";
    case opcode::NOT:
        return "not";
    case opcode::AND:
        return "and";
    case opcode::OR:
        return "or";
    case opcode::XOR:
        return "xor";
    case opcode::EQ:
        return "eq";
    case opcode::NEQ:
        return "neq";
    case opcode::MUX:
        return "mux";
    case opcode::LSH:
        return "lsh";
    case opcode::RSH:
        return "rsh";
    case opcode::ARSH:
        return "arsh";
    case opcode::MSK:
        return "msk";
    case opcode::CAT:
        return "cat";
    case opcode::ADD:
        return "add";
    case opcode::SUB:
        return "sub";
    case opcode::LT:
        return "lt";
    case opcode::GTE:
        return "gte";
    case opcode::MUL:
        return "mul";
    case opcode::LOG2:
        return "log2";
    case opcode::LD:
        return "ld";
    case opcode::ST:
        return "st";
    case opcode::LDI:
        return "ldi";
    case opcode::STI:
        return "sti";
    }

    fprintf(stderr, "std::to_string(hurricane_bfd::opcode) overran switch\n");
    fprintf(stderr, "  This is frequently a compile corruption problem\n");
    abort();
}

enum opcode hurricane_bfd::to_opcode(const std::string& str)
{
    for (const auto& opcode: all_opcodes)
        if (strcmp(str.c_str(), std::to_string(opcode).c_str()) == 0)
            return opcode;

    std::fprintf(stderr, "Unable to convert '%s' to an opcode\n",
                 str.c_str()
        );
    abort();
}
