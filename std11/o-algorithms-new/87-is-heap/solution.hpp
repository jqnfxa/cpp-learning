#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

/*
 * Step 87 -- std::is_heap / is_heap_until. See task.md for the spec.
 *
 * Implement a max-heap validity check and a first-violator index.
 */

namespace task87
{

/* TODO: return std::is_heap(v.begin(), v.end()); */
inline bool is_max_heap(const std::vector<int> & /*v*/)
{
    return false;
}

/* TODO: auto it = std::is_heap_until(v.begin(), v.end());
 *       return static_cast<std::size_t>(it - v.begin()); */
inline std::size_t first_heap_violator(const std::vector<int> & /*v*/)
{
    return 0;
}

}
