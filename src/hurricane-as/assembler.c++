/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of dreamer-binutils.
 * 
 * dreamer-binutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * dreamer-binutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with dreamer-binutils.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "assembler.h++"
#include "label.h++"
#include <cstring>
#include <sstream>
#include <map>
#include <regex>
using namespace hurricane_as;

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

hurricane_bfd::object::ptr assembler::read_as_file(const std::string& filename)
{
    auto file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open '%s'\n", filename.c_str());
        return NULL;
    }

    auto out = assembler::read_as_file(file);

    fclose(file);
    return out;
}

hurricane_bfd::object::ptr assembler::read_as_file(FILE* file)
{
    if (file == NULL)
        return NULL;

    /* The current offset, which starts out at 0. */
    auto label = std::make_shared<hurricane_as::label>("");
    auto offset = label->base();
    std::string section = "";

    /* Some lookup tables to use later when attempting to calculate
     * addresses. */
    std::vector<std::pair<std::string, hurricane_as::label::offset::ptr>> bundle_offset_pairs;
    std::map<std::string, hurricane_as::label::ptr> name2label;
    std::map<hurricane_as::label::offset::ptr, std::string> offset2bundle;
    std::map<hurricane_as::label::offset::ptr, std::string> offset2section;
    std::map<hurricane_as::label::offset::ptr, hurricane_as::label::ptr> offset2label;
    std::map<std::pair<int, int>, hurricane_bfd::tile::ptr> addr2tile;
    std::map<std::string, hurricane_bfd::tile::ptr> section2tile;
    std::vector<hurricane_bfd::tile::ptr> tiles;

    /* The first assembler pass just calculates the offset of every
     * label.  This is easy to do, because our bundles are always of a
     * fixed size. */
    char line_buf[BUFFER_SIZE];
    while (fgets(line_buf, BUFFER_SIZE, file) != NULL) {
        /* First strip the line of all leading and trailing
         * whitespace. */
        char* line = line_buf;
        while ((strlen(line) > 0) && isspace(line[strlen(line)-1]))
            line[strlen(line)-1] = '\0';
        while (isspace(*line))
            line++;

        /* Then check to see what sort of line this is. */
        if (strcmp(line, "") == 0) {
        } else if (std::regex_match(line, std::regex(".*:"))) {
            /* This type of line contains a label. */
            auto name = std::string(line, 0, strlen(line)-1);
            label = std::make_shared<hurricane_as::label>(name, offset);
            offset = label->base();
            name2label[name] = label;
        } else if (std::regex_match(line, std::regex("\\{ .* \\}"))) {
            /* This type of line contains a bundle, which needs to
             * change the current offset in additon to building some
             * maps that allow one to look up the current offset for
             * later use. */
            offset2bundle[offset] = line;
            offset2section[offset] = section;
            offset2label[offset] = label;
            bundle_offset_pairs.push_back(std::make_pair(line, offset));
            offset = offset->increment();
        } else if (std::regex_match(line, std::regex("section .*"))) {
            section = std::string(line + strlen("section "));

            char t[BUFFER_SIZE];
            int x, y;
            if (sscanf(line, "section .%[^'_']__tile_%d__%d", t, &x, &y) == 3) {
                auto pair = std::make_pair(x, y);
                auto l = addr2tile.find(pair);
                if (l == addr2tile.end()) {
                    hurricane_bfd::tile_address a(x, y);
                    auto tile = std::make_shared<hurricane_bfd::tile>(a);
                    addr2tile[pair] = tile;
                    tiles.push_back(tile);
                    l = addr2tile.find(pair);
                }
                section2tile[section] = l->second;

                label = std::make_shared<hurricane_as::label>("");
                offset = label->base();
            } else {
                fprintf(stderr, "Unable to demangle section '%s'\n",
                        section.c_str()
                    );
                abort();
            }
        } else if (std::regex_match(line, std::regex("global .*"))) {
            /* This does nothing, it's really there for linker stuff
             * but we don't link yet. */
        } else {
            fprintf(stderr, "Unable to parse line: '%s'\n", line);
            return NULL;
        }
    }

    /* Walk through every bundle and try to turn it into an actual
     * instruction. */
    for (const auto& pair: bundle_offset_pairs) {
        auto offset = pair.second;

        /* Split this bundle up into individual instructions. */
        auto i_strings = [&]()
            {
                auto b_string = std::string(pair.first,
                                            2,
                                            strlen(pair.first.c_str()) - 4
                    );


                auto trim = [](const std::string& str)
                {
                    std::size_t first = str.find_first_not_of(' ');
                    std::size_t last  = str.find_last_not_of(' ');
                    return str.substr(first, last-first+1);
                };

                std::istringstream iss(b_string);

                std::vector<std::string> out;
                {
                    std::string token;
                    while (std::getline(iss, token, ';'))
                        out.push_back(trim(token));
                }

                return out;
            }();

        std::vector<hurricane_bfd::instruction::ptr> instructions;
        for (const auto& i_string: i_strings) {
            std::string opcode_string;
            std::vector<std::string> operand_strings;
            {
                std::istringstream split(i_string);
                split >> opcode_string;
                while (split) {
                    std::string str;
                    split >> str;
                    /* FIXME: Why is this happening? */
                    if (strlen(str.c_str()) > 0)
                        operand_strings.push_back(str);
                }
            }

            auto opcode = hurricane_bfd::to_opcode(opcode_string);
            std::vector<hurricane_bfd::operand::ptr> operands(
                operand_strings.size()
                );
            std::transform(
                operand_strings.begin(),
                operand_strings.end(),
                operands.begin(),
                [](std::string op) -> hurricane_bfd::operand::ptr
                {
                    if (op.c_str()[0] == 'x') {
                        auto index = atoi(op.c_str() + 1);
                        return hurricane_bfd::operand_reg::index(index);
                    }

                    for (const auto& dir: hurricane_bfd::all_directions)
                        if (strcmp(std::to_string(dir).c_str(), op.c_str()) == 0)
                            return hurricane_bfd::operand_net::direction(dir);

                    unsigned int lit;
                    if (sscanf(op.c_str(), "%u", &lit) == 1)
                        return hurricane_bfd::operand_lit::uint(lit);

                    fprintf(stderr, "Unable to parse operand '%s'\n",
                            op.c_str());
                    abort();
                    return NULL;
                }
                );

            instructions.push_back(
                std::make_shared<hurricane_bfd::instruction>(
                    opcode,
                    operands
                    )
                );
        }

        auto bundle = std::make_shared<hurricane_bfd::bundle>(instructions);

        auto section = [&]()
            {
                auto l = offset2section.find(offset);
                if (l == offset2section.end()) {
                    fprintf(stderr, "Unable to find section\n");
                    abort();
                }
                return l->second;
            }();

        auto tile = [&]()
            {
                auto l = section2tile.find(section);
                if (l == section2tile.end()) {
                    fprintf(stderr, "Unable to find tile for '%s'\n",
                            section.c_str());
                    abort();
                }
                return l->second;
            }();

        auto label = [&]()
            {
                auto l = offset2label.find(offset);
                if (l == offset2label.end()) {
                    fprintf(stderr, "Unable to find label offset\n");
                    abort();
                }
                return l->second;
            }();

        if (std::regex_match(label->name(), std::regex("_lo_.*")))
            tile->add_lo(bundle);
        else if (std::regex_match(label->name(), std::regex("_hi_.*")))
            tile->add_hi(bundle);
        else {
            fprintf(stderr, "Unable to demangle label '%s'\n",
                    label->name().c_str()
                );
            abort();
        }
    }

    return std::make_shared<hurricane_bfd::object>(tiles);
}
