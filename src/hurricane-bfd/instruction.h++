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

#include "direction.h++"
#include "opcode.h++"
#include "operand.h++"
#include "maybe.h++"

namespace hurricane_bfd {
    /* Stores a single instruction. */
    class instruction {
    public:
        typedef std::shared_ptr<instruction> ptr;
        friend class std::shared_ptr<instruction>;

    private:
        enum opcode _opcode;
        std::shared_ptr<operand> _d;
        std::shared_ptr<operand> _x;
        std::shared_ptr<operand> _y;
        std::shared_ptr<operand> _z;

    public:
        /* A bunch of different constructors that represent the
         * different instruction formats.  Note that you're probably
         * not going to want to actually use these, as they're a bit
         * dangerous. */
        instruction(enum opcode op);
        instruction(enum opcode op,
                    const operand::ptr& d);
        instruction(enum opcode op,
                    const operand::ptr& d,
                    const operand::ptr& x);
        instruction(enum opcode op,
                    const operand::ptr& d,
                    const operand::ptr& x,
                    const operand::ptr& y);
        instruction(enum opcode op,
                    const operand::ptr& d,
                    const operand::ptr& x,
                    const operand::ptr& y,
                    const operand::ptr& z);
        instruction(enum opcode op,
                    const std::vector<operand::ptr>& ops);

    public:
        /* Returns all the operands in order, including the
         * destination. */
        std::vector<operand::ptr> operands(void) const;

        /* Returns the input operands, in some sort of order. */
        std::vector<operand::ptr> inputs(void) const;

        /* Either returns FALSE, which indicates there is no
         * destination, or a destination operand. */
        maybe<operand::ptr> d(void) const;

        /* Inputs to binary operations. */
        maybe<operand::ptr> x(void) const;
        maybe<operand::ptr> y(void) const;

        /* Returns the select signal, which is only valid for MUX. */
        maybe<operand::ptr> sel(void) const;
        maybe<operand::ptr> hi(void) const;
        maybe<operand::ptr> lo(void) const;

        /* Returns the base and offset used for memory operations. */
        maybe<operand::ptr> base(void) const;
        maybe<operand::ptr> offset(void) const;

        /* Either returns FALSE, which indicates that there is no mask
         * on this operation, or an integer, which consists of the
         * mask. */
        maybe<operand::ptr> mask(void) const;

        /* Formats this instruction as a string, in multiple different
         * formats. */
        std::string jrb_string(void) const;
        std::string as_string(void) const;
        uint32_t bits(void) const;

    public:
        /* These are kind of odd.  You probably don't want to be using
         * them, but instead want to make a bundle from a HEX file
         * line. */
        static ptr parse_hex_alu(const std::string hex,
                                 enum direction allow_out_dir,
                                 bool allow_reg);
        static ptr parse_hex_net(const std::string hex,
                                 enum direction allow_out_dir);
    };
}

#endif
