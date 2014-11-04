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

/* The different DREAMER binary instruction formats. */
union inst {
    uint32_t bits;

    /* These bits are true for all instruction encoding formats, so
     * you can always look at them!  This matches the definiton of the
     * FixInst from Scala. */
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

    /* The format used for "sti". */
    struct {
        unsigned int out : 4;
        unsigned int in  : 2;
        unsigned int off : 10;
        unsigned int en  : 1;
        unsigned int xi  : 5;
        unsigned int unu : 5;
        unsigned int op  : 5;
    } sti __attribute__((packed));

    /* The format used for "ldi". */
    struct {
        unsigned int out : 4;
        unsigned int in  : 2;
        unsigned int off : 10;
        unsigned int en  : 1;
        unsigned int unu : 5;
        unsigned int di  : 5;
        unsigned int op  : 5;
    } ldi __attribute__((packed));
};
typedef union inst inst_t;

/* Code to convert a single instruction from a HEX string to a bit
 * string. */
static inst_t to_inst(const std::string hex);

/* Deals with the fact that some registers don't have a destination
 * register. */
static size_t opmap(enum opcode op, size_t index);

instruction::instruction(enum opcode op)
    : _opcode(op),
      _d(NULL),
      _x(NULL),
      _y(NULL),
      _z(NULL)
{
}

instruction::instruction(enum opcode op,
                         const operand::ptr& d)
    : _opcode(op),
      _d(d),
      _x(NULL),
      _y(NULL),
      _z(NULL)
{
}

instruction::instruction(enum opcode op,
                         const operand::ptr& d,
                         const operand::ptr& x)
    : _opcode(op),
      _d(d),
      _x(x),
      _y(NULL),
      _z(NULL)
{
}

instruction::instruction(enum opcode op,
                         const operand::ptr& d,
                         const operand::ptr& x,
                         const operand::ptr& y)
    : _opcode(op),
      _d(d),
      _x(x),
      _y(y),
      _z(NULL)
{
}

instruction::instruction(enum opcode op,
                         const operand::ptr& d,
                         const operand::ptr& x,
                         const operand::ptr& y,
                         const operand::ptr& z)
    : _opcode(op),
      _d(d),
      _x(x),
      _y(y),
      _z(z)
{
}

instruction::instruction(enum opcode op,
                         const std::vector<operand::ptr>& ops)
    : _opcode(op),
      _d(ops.size() > opmap(op, 0) ? ops[opmap(op, 0)] : NULL),
      _x(ops.size() > opmap(op, 1) ? ops[opmap(op, 1)] : NULL),
      _y(ops.size() > opmap(op, 2) ? ops[opmap(op, 2)] : NULL),
      _z(ops.size() > opmap(op, 3) ? ops[opmap(op, 3)] : NULL)
{
}

std::vector<operand::ptr> instruction::operands(void) const
{
    switch (_opcode) {
    case opcode::NO:
        return {};

    case opcode::LIT:
        return {_d, _x};

    case opcode::CAT:
        return {_d, _x, _y, _z};

    case opcode::LDI:
        return {_d, _x};

    case opcode::STI:
        return {_x, _y};

    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
        return {_d, _x, _y, _z};

    case opcode::LD:
        return {_d, _x, _y};

    case opcode::ST:
        return {_x, _y, _z};

    case opcode::MUX:
        return {_d, _x, _y, _z};

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::operands() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return {};
}

std::vector<operand::ptr> instruction::inputs(void) const
{
    switch (_opcode) {
    case opcode::NO:
        return {};

    case opcode::LIT:
        return {_x};

    case opcode::LDI:
        return {_x};

    case opcode::STI:
        return {_x, _y};

    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::CAT:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
        return {_x, _y};

    case opcode::LD:
        return {_x, _y};

    case opcode::ST:
        return {_x, _y, _z};

    case opcode::MUX:
        return {_x, _y, _z};

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::operands() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return {};
}

maybe<operand::ptr> instruction::d(void) const
{
    switch (_opcode) {
    case opcode::NO:
    case opcode::STI:
    case opcode::ST:
        return maybe<operand::ptr>();

    case opcode::MUX:
    case opcode::LD:
    case opcode::LIT:
    case opcode::LDI:
    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::CAT:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
        return maybe<operand::ptr>(_d);

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::d() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::x(void) const
{
    switch (_opcode) {
    case opcode::NO:
    case opcode::STI:
    case opcode::ST:
    case opcode::MUX:
    case opcode::LD:
    case opcode::LIT:
    case opcode::LDI:
        return maybe<operand::ptr>();

    case opcode::ADD:
    case opcode::AND:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::SUB:
    case opcode::XOR:
    case opcode::ARSH:
    case opcode::LSH:
    case opcode::CAT:
    case opcode::RSH:
        return maybe<operand::ptr>(_x);

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::x() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::y(void) const
{
    if (x().valid() == true)
        return maybe<operand::ptr>(_y);

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::sel(void) const
{
    if (_opcode == opcode::MUX)
        return maybe<operand::ptr>(_x);

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::hi(void) const
{
    if (_opcode == opcode::MUX)
        return maybe<operand::ptr>(_y);

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::lo(void) const
{
    if (_opcode == opcode::MUX)
        return maybe<operand::ptr>(_z);

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::base(void) const
{
    switch (_opcode) {
    case opcode::ST:
    case opcode::LD:
        return maybe<operand::ptr>(_x);

    case opcode::STI:
    case opcode::LDI:
        return maybe<operand::ptr>();

    case opcode::ARSH:
    case opcode::LSH:
    case opcode::CAT:
    case opcode::RSH:
    case opcode::LIT:
    case opcode::MUX:
    case opcode::NO:
    case opcode::ADD:
    case opcode::AND:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::SUB:
    case opcode::XOR:
        return maybe<operand::ptr>();

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::x() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::offset(void) const
{
    switch (_opcode) {
    case opcode::ST:
    case opcode::LD:
    case opcode::STI:
    case opcode::LDI:
        return maybe<operand::ptr>(_y);

    case opcode::ARSH:
    case opcode::LSH:
    case opcode::CAT:
    case opcode::RSH:
    case opcode::LIT:
    case opcode::MUX:
    case opcode::NO:
    case opcode::ADD:
    case opcode::AND:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::SUB:
    case opcode::XOR:
        return maybe<operand::ptr>();

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::x() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return maybe<operand::ptr>();
}

maybe<operand::ptr> instruction::mask(void) const
{
    switch (_opcode) {
    case opcode::NO:
    case opcode::LIT:
    case opcode::LDI:
    case opcode::STI:
    case opcode::LD:
    case opcode::ST:
    case opcode::MUX:
        return maybe<operand::ptr>();

    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::CAT:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
        return _z == NULL ? maybe<operand::ptr>() : maybe<operand::ptr>(_z);

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::mask() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return maybe<operand::ptr>();
}

std::string instruction::jrb_string(void) const
{
    std::stringstream ss;

    if ((_opcode == opcode::ADD) && (_y->lit() == 0)) {
        if (_x->dir().valid() && _d->dir().valid())
            ss << std::to_string(_d) << " <- " << std::to_string(_x);
        else
            ss << std::to_string(_d) << " = " << std::to_string(_x) << " ";

        return ss.str();
    }

    if (_d != NULL)
        ss << std::to_string(_d) << " = ";

    ss << std::to_string(_opcode);
    if (mask().valid())
        ss << "'" << std::to_string(mask().value());
    ss << " ";

    for (const auto& input: inputs()) 
        ss << std::to_string(input) << " ";

    return ss.str();
}

uint32_t instruction::bits(void) const
{
    inst_t bits; bits.bits = 0;
    bits.inst.op = (int)_opcode;

    switch (_opcode) {
    case opcode::LIT:
        bits.lit.di = _d->idx().value();
        bits.lit.lit = _x->lit().value();
        return bits.bits;

    case opcode::NO:
    case opcode::LDI:
    case opcode::STI:
    case opcode::LD:
    case opcode::ST:
    case opcode::MUX:
    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::CAT:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::bits() unimplemented for %s\n",
                std::to_string(_opcode).c_str()
            );
        abort();
        break;
    }

    return -1;
}

std::string instruction::as_string(void) const
{
    std::stringstream ss;

    ss << std::to_string(_opcode);

    for (const auto& op: operands())
        if (op != NULL)
            ss << " " << std::to_string(op);

    return ss.str();
}

instruction::ptr instruction::parse_hex_alu(const std::string hex,
                                            enum direction allow_out_dir,
                                            bool allow_reg)
{
    auto bits = to_inst(hex);
    auto op = (enum opcode)(bits.inst.op);

    /* The general idea is that this gets called 5 times: once with
     * "allow_reg" to TRUE, and once with "allow_reg" to FALSE and
     * "allow_out_dir" to each NESW direction.  This allows the single
     * instruction to end up as many instructions if there's parallel
     * network movement. */
    auto in_reg_or_net = [&](int index) -> operand::ptr
        {
            if (index != 31)
                return operand_reg::index(index);

            return operand_net::direction((enum direction)bits.inst.in);
        };

    auto out_reg_or_net = [&](int index) -> operand::ptr
        {
            if (index != 31)
                return allow_reg ? operand_reg::index(index) : NULL;

            if (allow_reg)
                return NULL;

            if ((bits.inst.out & (1 << (int)allow_out_dir)) == 0)
                return NULL;

            return operand_net::direction(allow_out_dir);
        };

    auto y_reg_or_const = [&]() -> operand::ptr
        {
            if (bits.inst.iy == 1)
                return operand_lit::uint(bits.inst.yi);

            return in_reg_or_net(bits.inst.yi);
        };

    auto z_as_width = [&]() -> operand::ptr
        {
            if (bits.inst.zi == 0)
                return NULL;
            return operand_lit::uint(bits.inst.zi);
        };

    auto null_op = [&]() -> operand::ptr { return NULL; };

    /* This looks like a regular decoder: there's a format for every
     * sort of instruction we've got.  Unfortunately they're all kind
     * of different. */
    switch (op) {
    case opcode::NO:
        if (allow_reg == false)
            return NULL;

        return std::make_shared<instruction>(op);

    case opcode::LIT:
        if (out_reg_or_net(bits.inst.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(op,
                                             out_reg_or_net(bits.inst.di),
                                             operand_lit::uint(bits.lit.lit)
            );

    case opcode::CAT:
        if (out_reg_or_net(bits.inst.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(op,
                                             out_reg_or_net(bits.inst.di),
                                             in_reg_or_net(bits.inst.xi),
                                             y_reg_or_const(),
                                             operand_lit::uint(bits.inst.zi)
            );

    case opcode::LDI:
        if (out_reg_or_net(bits.ldi.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(op,
                                             out_reg_or_net(bits.ldi.di),
                                             operand_lit::uint(bits.ldi.off)
            );

    case opcode::STI:
        if (out_reg_or_net(bits.ldi.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(op,
                                             null_op(),
                                             out_reg_or_net(bits.sti.xi),
                                             operand_lit::uint(bits.sti.off)
            );

    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
        if (out_reg_or_net(bits.ldi.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(op,
                                             out_reg_or_net(bits.inst.di),
                                             in_reg_or_net(bits.inst.xi),
                                             y_reg_or_const(),
                                             z_as_width()
            );

    case opcode::LD:
        if (out_reg_or_net(bits.ldi.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(op,
                                             out_reg_or_net(bits.inst.di),
                                             in_reg_or_net(bits.inst.xi),
                                             y_reg_or_const()
            );

    case opcode::ST:
        if (allow_reg == false)
            return NULL;

        return std::make_shared<instruction>(op,
                                             null_op(),
                                             in_reg_or_net(bits.inst.xi),
                                             y_reg_or_const(),
                                             in_reg_or_net(bits.inst.zi)
            );

    case opcode::MUX:
        if (out_reg_or_net(bits.ldi.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(op,
                                             out_reg_or_net(bits.inst.di),
                                             in_reg_or_net(bits.inst.xi),
                                             y_reg_or_const(),
                                             in_reg_or_net(bits.inst.zi)
            );

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::parse_hex_alu() unimplemented for %s\n",
                std::to_string((enum opcode)(bits.inst.op)).c_str()
            );
        abort();
        break;
    }

    return NULL;
}

instruction::ptr instruction::parse_hex_net(const std::string hex,
                                            enum direction allow_out_dir)
{
    auto bits = to_inst(hex);
    auto op = (enum opcode)(bits.inst.op);

    /* This only gets called once for each direction, and exists
     * solely to output */
    auto in_net = [&](void) -> operand::ptr
        {
            return operand_net::direction((enum direction)bits.inst.in);
        };

    auto out_net = [&](int index) -> operand::ptr
        {
            if (index == 31)
                return NULL;

            if ((bits.inst.out & (1 << (int)allow_out_dir)) == 0)
                return NULL;

            return operand_net::direction(allow_out_dir);
        };

    /* This looks like a regular decoder: there's a format for every
     * sort of instruction we've got. */
    switch (op) {
    case opcode::NO:
    case opcode::LIT:
    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
    case opcode::STI:
    case opcode::LDI:
    case opcode::CAT:
    case opcode::LD:
    case opcode::ST:
    case opcode::MUX:
        if (out_net(bits.inst.di) == NULL)
            return NULL;

        return std::make_shared<instruction>(opcode::ADD,
                                             out_net(bits.inst.di),
                                             in_net(),
                                             operand_lit::uint(0)
            );

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "instruction::parse_hex_alu() unimplemented for %s\n",
                std::to_string((enum opcode)(bits.inst.op)).c_str()
            );
        abort();
        break;
    }

    return NULL;
}

inst_t to_inst(const std::string hex)
{
    inst_t bits;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> bits.bits;
    return bits;
}

size_t opmap(enum opcode op, size_t index)
{
    switch (op) {
    case opcode::ST:
    case opcode::STI:
        return index - 1;

    case opcode::NO:
    case opcode::LIT:
    case opcode::CAT:
    case opcode::LDI:
    case opcode::ADD:
    case opcode::AND:
    case opcode::ARSH:
    case opcode::EQ:
    case opcode::GTE:
    case opcode::LT:
    case opcode::LSH:
    case opcode::MUL:
    case opcode::NEQ:
    case opcode::OR:
    case opcode::RSH:
    case opcode::SUB:
    case opcode::XOR:
    case opcode::LD:
    case opcode::MUX:
        return index;

    case opcode::RST:
    case opcode::RND:
    case opcode::EAT:
    case opcode::NOT:
    case opcode::MSK:
    case opcode::LOG2:
        fprintf(stderr, "opmap() unimplemented for %s\n",
                std::to_string(op).c_str()
            );
        abort();
        break;
    }

    return -1;
}
