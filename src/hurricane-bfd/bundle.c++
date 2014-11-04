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

#include "bundle.h++"
#include "instruction_alu.h++"
#include "instruction_net.h++"
#include <sstream>
using namespace hurricane_bfd;

bundle::bundle(const instruction::ptr& a)
    : _instructions({a})
{
}


bundle::bundle(const instruction::ptr& a, const instruction::ptr& b)
    : _instructions({a, b})
{
}

bundle::ptr bundle::parse_hex(const std::string hex)
{
    auto alu = instruction_alu::parse_hex(hex);
    auto net = instruction_net::parse_hex(hex);

    if ((alu == NULL) && (net == NULL))
        return NULL;
    if ((alu != NULL) && (net == NULL))
        return std::make_shared<bundle>(alu);
    if ((alu != NULL) && (net != NULL))
        return std::make_shared<bundle>(alu, net);
    if ((alu == NULL) && (net != NULL))
        return std::make_shared<bundle>(net);

    return NULL;
}

std::string bundle::jrb_string(void) const
{
    if (instructions().size() == 1)
        return instructions()[0]->jrb_string() + " ";

    return instructions()[0]->jrb_string() + " ; " + instructions()[1]->jrb_string();
}

std::string bundle::as_string(void) const
{
    if (instructions().size() == 1)
        return instructions()[0]->as_string();

    std::stringstream ss;
    ss << "{ ";
    bool first = true;
    for (const auto& instruction: instructions()) {
        if (first)
            ss << instruction->as_string();
        else
            ss << " ; " << instruction->as_string();
        first = false;
    }
    ss << " }";

    return ss.str();
}
