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

#include "version.h"
#include <hurricane-bfd/object.h++>
#include <cstdio>
#include <cstring>
#include <unistd.h>

int main(int argc, const char **argv)
{
    if ((argc == 1) || (strcmp(argv[1], "--help") == 0)) {
        fprintf(stderr,
                "hurricane-hexinfo <OBJECT.hex>: Prints HEX information\n"
            );
        return 1;
    }

    if ((argc == 2) && (strcmp(argv[1], "--version") == 0)) {
        printf("%s\n", PCONFIGURE_VERSION);
        return 0;
    }

    std::string input_filename = "";
    for (int i = 1; i < argc; ++i) {
        input_filename = argv[i];
    }

    if (strcmp(input_filename.c_str(), "") == 0) {
        fprintf(stderr, "Provide an input file name\n");
        abort();
    }

    if (access(input_filename.c_str(), R_OK) != 0) {
        fprintf(stderr, "Unable to open input file '%s'\n",
                input_filename.c_str()
            );
        abort();
    }

    auto object = hurricane_bfd::object::read_hex_file(input_filename);
    if (object == NULL) {
        fprintf(stderr, "HEX parsing failed for '%s'\n",
                input_filename.c_str()
            );
        abort();
    }

    for (const auto& tile: object->tiles()) {
        printf("Tile %s:\n", tile->address().name_without_space().c_str());
        printf("\tLow Instruction Count: %lu\n", tile->lo().size());
        printf("\tHigh Instruction Count: %lu\n", tile->hi().size());
    }

    return 0;
}
