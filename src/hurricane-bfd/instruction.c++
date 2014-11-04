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

#include "instruction.h++"
#include <algorithm> 
#include <cctype>
#include <cstring>
#include <functional> 
#include <locale>
#include <sstream>
using namespace hurricane_bfd;

instruction::instruction(const inst_t& bits)
    : _bits(bits),
      _has_debug(false)
{
}

instruction::instruction(const inst_t& bits, const std::string debug)
    : _bits(bits),
      _has_debug(true),
      _debug(debug)
{
}

bool instruction::sanity_check(void) const
{
    return true;
}

enum opcode instruction::op(void) const
{
    /* FIXME: While these opcodes currently have a direct mapping to
     * the DREAMER opcodes, this isn't actually correct -- we need a
     * little layer in between to make this code can all be
     * paramaterized correctly. */
    return (enum opcode)(_bits.inst.op);
}

instruction::reg_index_t instruction::d_index(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::LDI:
        return _bits.inst.di;

    case opcode::NO:
    case opcode::EAT:
    case opcode::ST:
    case opcode::STI:
        fprintf(stderr, "Requested D register index in %s, but there's no D\n",
                std::to_string(op()).c_str()
            );
        abort();
        break;
    }
    abort();
}

instruction::reg_index_t instruction::x_index(void) const
{
    switch (op()) {
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        return _bits.inst.xi;

    case opcode::NO:
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
        fprintf(stderr, "Requested X register index in %s, but there's no X\n",
                std::to_string(op()).c_str()
            );
        abort();
        break;

    case opcode::LIT:
        fprintf(stderr, "Requested X register index in %s, but there's is always immediate\n",
                std::to_string(op()).c_str()
            );
        abort();
        break;
    }
    abort();
}

instruction::reg_index_t instruction::y_index(void) const
{
    switch (op()) {
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        if (_bits.inst.iy == 0)
            return _bits.inst.yi;

        fprintf(stderr, "Requested Y register index, but Y is immediate\n");
        abort();
        break;

    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
        fprintf(stderr, "Requested Y register index in %s, but there's no Y\n",
                std::to_string(op()).c_str()
            );
        abort();
        break;
    }
    abort();
}

instruction::reg_index_t instruction::z_index(void) const
{
    switch (op()) {
    case opcode::MUX:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        return _bits.inst.zi;

    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
        fprintf(stderr, "Requested Z register index in %s, but there's no Z\n",
                std::to_string(op()).c_str()
            );
        abort();
        break;

    case opcode::LD:
        fprintf(stderr, "Requested Z register index in %s, but it's an implicit immediate\n",
                std::to_string(op()).c_str()
            );
        abort();
        break;
    }
    abort();
}

bool instruction::d_is_register(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::LDI:
        return (d_is_immediate() == false) && (d_index() != 31);

    case opcode::NO:
    case opcode::ST:
    case opcode::STI:
        return false;
    }
    abort();
}

bool instruction::x_is_register(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        return (x_is_immediate() == false) && (x_index() != 31);

    case opcode::NO:
        return false;

    case opcode::LDI:
        return false;

    case opcode::STI:
        return true;
    }
    abort();
}

bool instruction::y_is_register(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        return (y_is_immediate() == false) && (y_index() != 31);

    case opcode::NO:
    case opcode::LIT:
    case opcode::LDI:
    case opcode::STI:
        return false;
    }
    abort();
}

bool instruction::z_is_register(void) const
{
    switch (op()) {
    case opcode::MUX:
    case opcode::ST:
        return (z_is_immediate() == false) && (z_index() != 31);

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::NO:
    case opcode::LIT:
    case opcode::AND:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::LDI:
    case opcode::STI:
        return false;

    }
    abort();
}

instruction::immediate_t instruction::d_imm(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        fprintf(stderr, "There are no D immediates\n");
        abort();
        break;
    }
    abort();
}

instruction::immediate_t instruction::x_imm(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::STI:
        fprintf(stderr, "Requested X as immediate, but it's a register index\n");
        abort();
        break;

    case opcode::LIT:
        return _bits.lit.lit;

    case opcode::LDI:
        return _bits.ldi.off;
    }
    abort();
}

instruction::immediate_t instruction::y_imm(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        if (_bits.inst.iy == 1)
            return _bits.inst.yi;

        fprintf(stderr, "Requested Y immediate, but Y is a register index\n");
        abort();

        break;

    case opcode::LDI:
        fprintf(stderr, "Requested Y immediate, but there are no Y immediates\n");
        abort();
        break;

    case opcode::STI:
        return _bits.sti.off;
    }
    abort();
}

instruction::immediate_t instruction::z_imm(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        fprintf(stderr, "Requested Z, but not valid for this format\n");
        abort();
        break;

    case opcode::CAT:
        return _bits.inst.zi;

    }
    abort();
}


bool instruction::d_is_immediate(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        return false;
        break;
    }
    abort();
}

bool instruction::x_is_immediate(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::STI:
        return false;

     case opcode::LIT:
         return true;

    case opcode::LDI:
        return true;
    }
    abort();
}

bool instruction::y_is_immediate(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        return _bits.inst.iy;

    case opcode::LIT:
        return false;

    case opcode::LDI:
        return false;

    case opcode::STI:
        return true;
    }
    abort();
}

bool instruction::z_is_immediate(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        return false;

    case opcode::LDI:
    case opcode::STI:
        return false;
    }
    abort();
}

std::map<enum direction, bool> instruction::d_net(void) const
{
    std::map<enum direction, bool> out;
    for (const auto& direction: all_directions)
        out[direction] = false;

    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
    {
        /* FIXME: Here is some logic that depends on explicit register
         * indicies, while we should really be dealing with this in a
         * paramaterizable way. */
        for (int i = 0; i < 4; ++i)
            if ((_bits.inst.out & (1 << i)) != 0)
                out[(enum direction)i] = true;
        break;
    }
    }

    return out;
}

std::map<enum direction, bool> instruction::x_net(void) const
{
    std::map<enum direction, bool> out;
    for (const auto& direction: all_directions)
        out[direction] = false;

    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        /* FIXME: Here is some logic that depends on explicit register
         * indicies, while we should really be dealing with this in a
         * paramaterizable way. */
        if (x_is_network() == true)
            out[(enum direction)(_bits.inst.in)] = true;
        break;
    }

    return out;
}

std::map<enum direction, bool> instruction::y_net(void) const
{
    std::map<enum direction, bool> out;
    for (const auto& direction: all_directions)
        out[direction] = false;

    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        /* FIXME: Here is some logic that depends on explicit register
         * indicies, while we should really be dealing with this in a
         * paramaterizable way. */
        if (y_is_network() == true)
            out[(enum direction)(_bits.inst.in)] = true;
        break;

    case opcode::LDI:
    case opcode::STI:
        fprintf(stderr, "Requested y_net in %s, but that can't touch network\n",
                std::to_string(op()).c_str()
            );
        abort();
    }

    return out;
}

std::map<enum direction, bool> instruction::z_net(void) const
{
    std::map<enum direction, bool> out;
    for (const auto& direction: all_directions)
        out[direction] = false;

    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        /* FIXME: Here is some logic that depends on explicit register
         * indicies, while we should really be dealing with this in a
         * paramaterizable way. */
        if (z_is_network() == true)
            out[(enum direction)(_bits.inst.in)] = true;
        break;

    case opcode::LDI:
    case opcode::STI:
        fprintf(stderr, "Requested z_net in %s, but that can't touch network\n",
                std::to_string(op()).c_str()
            );
        abort();
    }

    return out;
}

bool instruction::d_is_network(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::LDI:
        return (d_is_immediate() == false) && (d_index() == 31);

    case opcode::NO:
    case opcode::ST:
    case opcode::STI:
        return false;
    }
    abort();
}

bool instruction::x_is_network(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        return (x_is_immediate() == false) && (x_index() == 31);

    case opcode::NO:
        return false;
    }
    abort();
}

bool instruction::y_is_network(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::CAT:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        return (y_is_immediate() == false) && (y_index() == 31);

    case opcode::NO:
    case opcode::LIT:
    case opcode::LDI:
    case opcode::STI:
        return false;
    }
    abort();
}

bool instruction::z_is_network(void) const
{
    switch (op()) {
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MUX:
        return (z_is_immediate() == false) && (z_index() == 31);

    case opcode::NO:
    case opcode::LIT:
    case opcode::LDI:
    case opcode::STI:
    case opcode::CAT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
    case opcode::MSK:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::MUL:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
        return false;
    }
    abort();
}

instruction::width_t instruction::d_width(void) const
{
    fprintf(stderr, "d cannot be width\n");
    abort();
}

instruction::width_t instruction::x_width(void) const
{
    fprintf(stderr, "x cannot be width\n");
    abort();
}

instruction::width_t instruction::y_width(void) const
{
    fprintf(stderr, "y cannot be width\n");
    abort();
}

instruction::width_t instruction::z_width(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::MSK:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        fprintf(stderr, "Requested Z as width, but not valid for this format\n");
        abort();
        break;

    case opcode::MUL:
    case opcode::CAT:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
        return _bits.inst.zi;

    }
    abort();
}

bool instruction::d_is_width(void) const
{
    return false;
}

bool instruction::x_is_width(void) const
{
    return false;
}

bool instruction::y_is_width(void) const
{
    return false;
}

bool instruction::z_is_width(void) const
{
    switch (op()) {
    case opcode::NO:
    case opcode::RST:
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::AND:
    case opcode::OR:
    case opcode::XOR:
    case opcode::EQ:
    case opcode::NEQ:
    case opcode::MUX:
    case opcode::MSK:
    case opcode::ADD:
    case opcode::SUB:
    case opcode::LT:
    case opcode::GTE:
    case opcode::LOG2:
    case opcode::LD:
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        return false;

    case opcode::CAT:
        return true;

    case opcode::MUL:
    case opcode::LSH:
    case opcode::RSH:
    case opcode::ARSH:
        return z_width() != 0;
    }
    abort();
}

bool instruction::parallel_network(void) const
{
    if (d_is_network())
        return false;

    for (const auto& dir: parallel_net_out())
        if (dir.second == true)
            return true;

    return false;
}

std::map<enum direction, bool> instruction::parallel_net_out(void) const
{
    std::map<enum direction, bool> out;
    for (const auto& direction: all_directions)
        out[direction] = false;

    for (int i = 0; i < 4; ++i)
        if ((_bits.inst.out & (1 << i)) != 0)
            out[(enum direction)i] = true;

    return out;
}

std::map<enum direction, bool> instruction::parallel_net_in(void) const
{
    std::map<enum direction, bool> out;
    for (const auto& direction: all_directions)
        out[direction] = false;

    out[(enum direction)(_bits.inst.in)] = true;

    return out;
}
