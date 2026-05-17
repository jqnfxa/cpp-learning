#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

/*
 * Step 86 -- std::is_sorted / is_sorted_until. See task.md for the spec.
 *
 * Implement an ascending-sortedness check and a first-descent index.
 */

namespace task86
{

/* TODO: return std::is_sorted(v.begin(), v.end()); */
inline bool is_sorted_asc(const std::vector<int> & /*v*/)
{
    return false;
}

/* TODO: auto it = std::is_sorted_until(v.begin(), v.end());
 *       return static_cast<std::size_t>(it - v.begin()); */
inline std::size_t first_descent(const std::vector<int> & /*v*/)
{
    return 0;
}

}
