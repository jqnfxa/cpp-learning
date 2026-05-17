#pragma once

#include <cstddef>
#include <forward_list>
#include <vector>

/*
 * Step 74 -- std::forward_list. See task.md for the spec.
 *
 * Implement insert_at / erase_at / length / to_vector using
 * before_begin() + insert_after / erase_after.
 */

namespace task74
{

using list = std::forward_list<int>;

/* TODO: walk before_begin forward `index` times, then insert_after. */
inline void insert_at(list & /*l*/, std::size_t /*index*/, int /*value*/)
{
}

/* TODO: walk before_begin forward `index` times, then erase_after. */
inline void erase_at(list & /*l*/, std::size_t /*index*/)
{
}

/* TODO: walk and count. forward_list has no constant-time size(). */
inline std::size_t length(const list & /*l*/)
{
    return 0;
}

/* TODO: return std::vector<int>(l.begin(), l.end()). */
inline std::vector<int> to_vector(const list & /*l*/)
{
    return {};
}

}
