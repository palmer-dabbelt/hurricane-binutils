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

#ifndef HURRICANE_BFD__OPERAND_HXX
#define HURRICANE_BFD__OPERAND_HXX

#include "direction.h++"
#include "maybe.h++"
#include <memory>
#include <string>

namespace hurricane_bfd {
    /* A generic class that represents all the different types of
     * operands that are supported by BFD. */
    class operand {
    public:
        typedef std::shared_ptr<operand> ptr;

        virtual std::string to_string(void) const = 0;

        /* Returns FALSE if this is not a literal, or the value if it
         * is a literal. */
        virtual maybe<int64_t> lit(void) const = 0;

        /* Returns FALSE if this is not a register index, or the
         * register index. */
        virtual maybe<unsigned int> idx(void) const = 0;

        /* Returns FALSE if this is not a network port, or the
         * direction if it is. */
        virtual maybe<enum direction> dir(void) const = 0;
    };

    /* The rest of these are just specific implementations of the
     * operand class.  You probably shouldn'd need to look at anything
     * but the constructors, they're just all in the header file
     * because they're so small. */
    class operand_reg: public operand {
    public:
        typedef std::shared_ptr<operand_reg> ptr;
        friend class std::shared_ptr<operand_reg>;

    private:
        const unsigned int _idx;

    public:
        operand_reg(unsigned int idx)
            : _idx(idx)
            {  }

        std::string to_string(void) const
            { return std::string("x") + std::to_string(_idx); }

        maybe<int64_t> lit(void) const { return maybe<int64_t>(); }
        maybe<unsigned int> idx(void) const { return maybe<unsigned int>(_idx); }
        maybe<enum direction> dir(void) const { return maybe<enum direction>(); }

        static ptr index(unsigned int idx)
            { return std::make_shared<operand_reg>(idx); }
    };

    class operand_net: public operand {
    public:
        typedef std::shared_ptr<operand_net> ptr;

    private:
        const enum direction _dir;

    public:
        operand_net(enum direction dir)
            : _dir(dir)
            {  }

        std::string to_string(void) const
            { return std::to_string(_dir); }

        maybe<int64_t> lit(void) const { return maybe<int64_t>(); }
        maybe<unsigned int> idx(void) const { return maybe<unsigned int>(); }
        maybe<enum direction> dir(void) const { return maybe<enum direction>(_dir); }

        static ptr direction(enum direction dir)
            { return std::make_shared<operand_net>(dir); }
    };

    class operand_lit: public operand {
    public:
        typedef std::shared_ptr<operand_lit> ptr;

    private:
        const int64_t _value;

    public:
        operand_lit(int64_t value)
            : _value(value)
            {  }

        std::string to_string(void) const
            { return std::to_string(_value); }

        maybe<int64_t> lit(void) const { return maybe<int64_t>(_value); }
        maybe<unsigned int> idx(void) const { return maybe<unsigned int>(); }
        maybe<enum direction> dir(void) const { return maybe<enum direction>(); }

        static ptr uint(uint32_t value)
            { return std::make_shared<operand_lit>(value); }
    };
}

namespace std {
    string to_string(const hurricane_bfd::operand::ptr& op);
}

#endif
