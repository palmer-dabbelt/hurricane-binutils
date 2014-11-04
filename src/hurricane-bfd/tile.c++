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

#include "tile.h++"
using namespace hurricane_bfd;

static std::vector<bundle::ptr>
lo(const std::vector<bundle::ptr>& bundles,
   size_t count);

static std::vector<bundle::ptr>
hi(const std::vector<bundle::ptr>& bundles,
   size_t count);

tile::tile(const tile_address& addr,
           size_t lo_bundle_count,
           std::vector<bundle::ptr> bundles)
    : _addr(addr),
      _lo(::lo(bundles, lo_bundle_count)),
      _hi(::hi(bundles, lo_bundle_count))
{
}


std::vector<bundle::ptr>
lo(const std::vector<bundle::ptr>& bundles,
   size_t count)
{
    std::vector<bundle::ptr> out(count);
    if (count > bundles.size()) {
        fprintf(stderr, "lo_count greater than bundle count: %llu %llu\n",
                (long long unsigned)count,
                (long long unsigned)bundles.size()
            );
        abort();
    }

    for (size_t i = 0; i < count; ++i)
        out[i] = bundles[i];

    return out;
}

std::vector<bundle::ptr>
hi(const std::vector<bundle::ptr>& bundles,
   size_t count)
{
    std::vector<bundle::ptr> out(bundles.size() - count);

    for (size_t i = count; i < bundles.size(); ++i)
        out[i - count] = bundles[i];

    return out;
}
