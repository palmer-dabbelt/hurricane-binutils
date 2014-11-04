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

#include <hurricane-as/assembler.h++>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>

static void print_help_text(FILE *out);

int main(int argc, const char **argv)
{
    if (argc == 1) {
        print_help_text(stderr);
        abort();
    }

    std::string input_filename = "";
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help_text(stderr);
            return 0;
        } else {
            input_filename = argv[i];
        }
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

    auto object = hurricane_as::assembler::read_as_file(input_filename);
    if (object == NULL) {
        fprintf(stderr,  "Unable to assemble '%s'\n",
                input_filename.c_str()
            );
        abort();
    }

    object->write_hex_file(stdout);
    return 0;
}


void print_help_text(FILE *out)
{
    fprintf(out, "hurricane-as <PROGRAM.as>: Assembles PROGRAM\n");
    fprintf(out, "  --help: Prints this help text and exits\n");
}
