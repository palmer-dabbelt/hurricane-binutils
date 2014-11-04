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

#include "instruction_net.h++"
#include <algorithm> 
#include <cctype>
#include <cstring>
#include <functional> 
#include <locale>
#include <sstream>
using namespace hurricane_bfd;

instruction_net::instruction_net(const inst_t& bits)
    : instruction(bits)
{
}

instruction_net::instruction_net(const inst_t& bits, const std::string debug)
    : instruction(bits, debug)
{
}

bool instruction_net::sanity_check(void) const
{
    if (this->parallel_network() == false)
        return false;
    return true;
}

std::string instruction_net::jrb_string(void) const
{
    std::stringstream ss;

    ss << std::to_string(parallel_net_out());
    ss << " <- ";
    ss << std::to_string(parallel_net_in());

    return ss.str();
}

std::string instruction_net::as_string(void) const
{
    std::stringstream ss;

    ss << " ; ";
    ss << std::to_string(parallel_net_out());
    ss << " <- ";
    ss << std::to_string(parallel_net_in());
    ss << " }";

    return ss.str();
}

instruction::ptr instruction_net::parse_hex(const std::string hex)
{
    auto i = instruction::parse_hex<instruction_net>(hex);
    return i;
}
