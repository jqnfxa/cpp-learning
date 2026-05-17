#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

/*
 * Step 60 -- auto for variables. See task.md.
 *
 * Implement two functions using `auto` for iterator and similar
 * verbose-type variables.
 */

namespace task60
{

/* TODO:
 *   int total = 0;
 *   for (auto it = v.begin(); it != v.end(); ++it) total += *it;
 *   return total;
 */
inline int sum_via_iterator(const std::vector<int> & /* v */)
{
    return 0;
}

/* TODO:
 *   auto it = std::find(v.begin(), v.end(), needle);
 *   return static_cast<std::size_t>(it - v.begin());
 */
inline std::size_t find_index(const std::vector<int> & /* v */, int /* needle */)
{
    return 0;
}

}
