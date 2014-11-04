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

#ifndef LIBHURRICANE_BFD__MAYBE_HXX
#define LIBHURRICANE_BFD__MAYBE_HXX

#include <cstdlib>

namespace hurricane_bfd {
    /* This is essentially a Unit tagged union, but  */
    template <class T>
    class maybe {
    private:
        const bool _valid;
        const T _value;

    public:
        maybe(void)
            : _valid(false),
              _value()
            {
            }

        maybe(const T& value)
            : _valid(true),
              _value(value)
            {
            }

    public:
        bool valid(void) const { return _valid; }
        T value(void) const
            {
                if (_valid == false)
                    abort();

                return _value;
            }

        bool operator==(T other)
            {
                if (_valid == false)
                    return false;

                return _value == other;
            }
    };
}

#endif
