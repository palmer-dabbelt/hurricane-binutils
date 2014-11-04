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

#ifndef HURRICANE_AS__LABEL_HXX
#define HURRICANE_AS__LABEL_HXX

#include <memory>
#include <string>

namespace hurricane_as {
    /* Stores a single label, which itself contains an absolute offset
     * and can be offset from. */
    class label {
    public:
        class offset {
        public:
            typedef std::shared_ptr<offset> ptr;

        private:
            uint64_t _absolute;
            uint64_t _relative;

        public:
            /* Creates an offset of 0. */
            offset(void);

            /* Creates an offset by manually specifying the absolute
             * and relative addresses. */
            offset(uint64_t absolute, uint64_t relative);

        public:
            /* Returns a new offset that has a relative part of 0. */
            ptr without_relative(void) const;

            /* Returns a new offfset that has been incremented by one
             * but retains the same base address. */
            ptr increment(void) const;
        };

    public:
        typedef std::shared_ptr<label> ptr;

    private:
        const std::string _name;
        const offset::ptr _base;

    public:
        label(std::string name);

        label(std::string name, offset::ptr base);

    public:
        std::string name(void) const { return _name; }
        const offset::ptr& base(void) const { return _base; }
    };
}

#endif
