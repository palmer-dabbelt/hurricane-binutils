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

#include "instruction_alu.h++"
#include <algorithm> 
#include <cctype>
#include <cstring>
#include <functional> 
#include <locale>
#include <sstream>
using namespace hurricane_bfd;

instruction_alu::instruction_alu(const inst_t& bits)
    : instruction(bits)
{
}

instruction_alu::instruction_alu(const inst_t& bits, const std::string debug)
    : instruction(bits, debug)
{
}

bool instruction_alu::sanity_check(void) const
{
    if ((op() == opcode::ADD) && y_is_immediate() && (y_imm() == 0))
        return !parallel_network();
    return true;
}

std::string instruction_alu::jrb_string(void) const
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

        return ss.str();
    }

    return ss.str();

print_and_abort:
    fprintf(stderr, "  Other decode: '%s'\n", ss.str().c_str());
    fprintf(stderr, "  HEX: 0x%X\n", bits().bits);
    abort();
}

std::string instruction_alu::as_string(void) const
{
    std::stringstream ss;

    /* First, write out the opcode. */
    ss << std::to_string(op());
    if (z_is_width())
        ss << "'" << z_width();

    /* If the destination is a register then output that register
     * to the string, formatted correctly. */
    if (d_is_register())
        ss << " " << "x" << std::to_string(d_index());
    if (d_is_network())
        ss << " " << std::to_string(d_net());

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

    return ss.str();
}

instruction::ptr instruction_alu::parse_hex(const std::string hex)
{
    auto i = instruction::parse_hex<instruction_alu>(hex);
    return i;
}
