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
    case opcode::LIT:
    case opcode::RND:
    case opcode::EAT:
        fprintf(stderr, "Requested X register index in %s, but there's no X\n",
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
        return (x_is_immediate() == false) && (x_index() != 31);
        break;
    }
    abort();
}

bool instruction::y_is_register(void) const
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
        return (y_is_immediate() == false) && (y_index() != 31);
        break;
    }
    abort();
}

bool instruction::z_is_register(void) const
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
        return (z_is_immediate() == false) && (z_index() != 31);
        break;
    }
    abort();
}

instruction::immediate_t instruction::d_imm(void) const
{
    switch (opcode()) {
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
    switch (opcode()) {
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
        fprintf(stderr, "There are no X immediates\n");
        abort();
        break;
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
    case opcode::LDI:
    case opcode::STI:
        if (_bits.inst.iy == 0)
            return _bits.inst.yi;

        fprintf(stderr, "Requested Y immediate, but Y is a register index\n");
        abort();

        break;
    }
    abort();
}

instruction::immediate_t instruction::z_imm(void) const
{
    switch (op()) {
    case opcode::LD:
        return _bits.inst.zi;
        break;

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
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        fprintf(stderr, "There are no Z immediates\n");
        abort();
        break;
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

bool instruction::y_is_immediate(void) const
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
        return _bits.inst.iy;
        break;
    }
    abort();
}

bool instruction::z_is_immediate(void) const
{
    switch (op()) {
    case opcode::LD:
        return true;
        break;

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
    case opcode::ST:
    case opcode::LDI:
    case opcode::STI:
        return false;
        break;
    }
    abort();
}

std::map<enum direction, bool> instruction::net_x(void) const
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
        if (x_is_network() == true) {
            for (int i = 0; i < 4; ++i)
                if ((_bits.inst.out & (1 << i)) != 0)
                    out[(enum direction)i] = true;
        }
        break;
    }

    return out;
}

std::map<enum direction, bool> instruction::net_y(void) const
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
        if (y_is_network() == true) {
            for (int i = 0; i < 4; ++i)
                if ((_bits.inst.out & (1 << i)) != 0)
                    out[(enum direction)i] = true;
        }
        break;
    }

    return out;
}

std::map<enum direction, bool> instruction::net_z(void) const
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
        if (z_is_network() == true) {
            for (int i = 0; i < 4; ++i)
                if ((_bits.inst.out & (1 << i)) != 0)
                    out[(enum direction)i] = true;
        }
        break;
    }

    return out;
}

bool instruction::d_is_network(void) const
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
        return (d_is_immediate() == false) && (d_index() == 31);
        break;
    }
    abort();
}

bool instruction::x_is_network(void) const
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
        return (x_is_immediate() == false) && (x_index() == 31);
        break;
    }
    abort();
}

bool instruction::y_is_network(void) const
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
        return (y_is_immediate() == false) && (y_index() == 31);
        break;
    }
    abort();
}

bool instruction::z_is_network(void) const
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
        return (z_is_immediate() == false) && (z_index() == 31);
        break;
    }
    abort();
}

std::map<enum direction, bool> instruction::net_d(void) const
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
        for (int i = 0; i < 4; ++i)
            if ((_bits.inst.out & (1 << i)) != 0)
                out[(enum direction)(_bits.inst.in)] = true;
        break;
    }

    return out;
}

std::string instruction::to_string(void) const
{
    std::stringstream ss;

    auto netoutname = [&](void)
        {
            auto W = _bits.inst.out & 0x1 ? "W" : "";
            auto N = _bits.inst.out & 0x2 ? "N" : "";
            auto E = _bits.inst.out & 0x4 ? "E" : "";
            auto S = _bits.inst.out & 0x8 ? "S" : "";
            std::stringstream ss;
            ss << W << N << E << S;
            return ss.str();
        };

    auto netinname = [&](void)
        {
            switch (_bits.inst.in) {
            case 0: return "W";
            case 1: return "N";
            case 2: return "E";
            case 3: return "S";
            }
            abort();
        };

    auto netdest = [&](void) -> std::string
        {
            if (_bits.inst.di == 31)
                return netoutname();

            std::stringstream ss;
            ss << "x" << _bits.inst.di;
            return ss.str();
        };

    auto netsrc = [&](int reg) -> std::string
        {
            if (reg == 31)
                return netinname();

            std::stringstream ss;
            ss << "x" << reg;
            return ss.str();
        };

    auto netd = [&](void) { return netsrc(_bits.inst.di); };
    auto netx = [&](void) { return netsrc(_bits.inst.xi); };
    auto nety = [&](void) { 
        if (_bits.inst.iy == 0)
            return netsrc(_bits.inst.yi);
        else
            return std::to_string(_bits.inst.yi);
    };
    auto netz = [&](void) { return netsrc(_bits.inst.zi); };

    auto widthapos = [&](int width) -> std::string
        {
            if (width == 0)
                return "";

            std::stringstream ss;
            ss << "'" << width;
            return ss.str();
        };

    auto zw = [&](void) { return widthapos(_bits.inst.zi); };

    auto y_is_const_zero = [&](void)
        { return _bits.inst.iy == 1 && _bits.inst.yi == 0; };

    switch (op()) {
    case opcode::NO:
        ss << "nop";
        break;

    case opcode::LIT:
        ss << netdest() << " = lit " << _bits.lit.lit;
        break;

    case opcode::EQ:
        ss << netdest() << " = eq " << netx() << " " << nety();
        break;

    case opcode::MUX:
        ss << netdest() << " = mux " << netx() << " " << nety() << " " << netz();
        break;

    case opcode::RSH:
        ss << netdest() << " = rsh" << zw() << " " << netx() << " " << nety();
        break;

    case opcode::CAT:
        ss << netdest() << " = cat" << zw() << " " << netx() << " " << nety();
        break;

    case opcode::ADD:
        if (y_is_const_zero() == true)
            ss << netdest() << " = " << netx();
        else
            ss << netdest() << " = add " << netx() << " " << nety();
        break;

    case opcode::SUB:
        ss << netdest() << " = sub " << netx() << " " << nety();
        break;

    case opcode::MUL:
        ss << netdest() << " = mul" << zw() << " " << netx() << " " << nety();
        break;

    case opcode::LD:
        ss << netdest() << " = ld " << netx() << " " << nety();
        break;

    case opcode::ST:
        ss << "st " << netd() << " " << nety() << " " << netz();
        break;

    case opcode::LDI:
        ss << netdest() << " = ldi " << _bits.inst.xi;
        break;

    case opcode::STI:
        ss << "sti " << netx() << " " << _bits.inst.yi;
        break;

    default:
        fprintf(stderr, "Unhandled opcode: %s\n",
                std::to_string(op()).c_str()
            );
        abort();
        break;
    }

    if ((_bits.inst.di != 31) && (_bits.inst.out != 0))
        ss << " ; " << netoutname() << " <- " << netinname();
    else
        ss << " ";

    return ss.str();
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
