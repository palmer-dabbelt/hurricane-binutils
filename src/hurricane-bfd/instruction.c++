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

// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
static inline std::string rtrim(const std::string &s_in);

instruction::instruction(const inst_t& bits)
    : _bits(bits),
      _has_debug(false),
      _debug("")
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

std::string instruction::to_string(void) const
{
    std::stringstream ss;

    /* If the destination is a register then output that register
     * to the string, formatted correctly. */
    if (d_is_register())
        ss << "x" << std::to_string(d_index()) << " = ";
    if (d_is_network())
        ss << std::to_string(d_net()) << " = ";

    /* The opcode gets written directly out. */
    ss << std::to_string(op());
    if (z_is_width())
        ss << "'" << z_width();

    /* Check every opcode and output whatever exist. */
    if (x_is_immediate())
        ss << " " << std::to_string(x_imm());
    if (x_is_register())
        ss << " " << "x" << std::to_string(x_index());
    if (x_is_network())
        ss << " " << std::to_string(x_net());

    if (y_is_immediate())
        ss << " " << std::to_string(y_imm());
    if (y_is_register())
        ss << " " << "x" << std::to_string(y_index());
    if (y_is_network())
        ss << " " << std::to_string(y_net());

    if (z_is_immediate())
        ss << " " << std::to_string(z_imm());
    if (z_is_register())
        ss << " " << "x" << std::to_string(z_index());
    if (z_is_network())
        ss << " " << std::to_string(z_net());

    ss << " ";

    /* Check to see if there was a parallel network operation going
     * on. */
    if (parallel_network() == true) {
        ss << "; ";
        ss << std::to_string(parallel_net_out());
        ss << " <- ";
        ss << std::to_string(parallel_net_in());
    }

    /* This is a special format for NOPs, it's needed to match what
     * Jonathan is doing in his disassambler. */
    if ((op() == opcode::ADD) && y_is_immediate() && (y_imm() == 0)) {
        std::stringstream ss;

        if (d_is_network())
            ss << std::to_string(d_net());
        else if (d_is_immediate())
            ss << std::to_string(d_imm());
        else if (d_is_register())
            ss << "x" << std::to_string(d_index());
        else {
            fprintf(stderr, "D is not net, imm, or reg\n");
            goto print_and_abort;
        }

        ss << " = ";

        if (x_is_network())
            ss << std::to_string(x_net());
        else if (x_is_immediate())
            ss << std::to_string(x_imm());
        else if (x_is_register())
            ss << "x" << std::to_string(x_index());
        else {
            fprintf(stderr, "X is not net, imm, or reg\n");
            goto print_and_abort;
        }

        ss << " ";

        return ss.str();
    }

    return ss.str();

print_and_abort:
    fprintf(stderr, "  Other decode: '%s'\n", ss.str().c_str());
    fprintf(stderr, "  HEX: 0x%X\n", _bits.bits);
    abort();
}

instruction::ptr instruction::parse_hex(const std::string hex)
{
    auto hex2bits = [](const std::string hex)
        {
            inst_t bits;
            std::stringstream ss;
            ss << std::hex << hex;
            ss >> bits.bits;
            return bits;
        };

    auto bits = hex2bits(hex);

    if (strstr(hex.c_str(), " ") == NULL) {
        auto out = std::make_shared<instruction>(bits);
        if (out->sanity_check() == false)
            return NULL;
        return out;
    } else {
        std::string debug = rtrim(strstr(hex.c_str(), " ") + 1);
        auto out = std::make_shared<instruction>(bits, debug);
        if (out->sanity_check() == false)
            return NULL;
        return out;
    }
}

// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
std::string rtrim(const std::string &s_in)
{
    std::string s = s_in;
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
