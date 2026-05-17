#pragma once

#include <algorithm>
#include <vector>

/*
 * Step 80 -- std::all_of / any_of / none_of. See task.md for the spec.
 *
 * Implement three vector predicates using the C++11 algorithm trio
 * with simple lambda predicates.
 */

namespace task80
{

/* TODO: std::all_of(...) with [](int x) { return x > 0; }. */
inline bool is_all_positive(const std::vector<int> & /*v*/)
{
    return false;
}

/* TODO: std::any_of(...) with [](int x) { return x == 0; }. */
inline bool has_zero(const std::vector<int> & /*v*/)
{
    return false;
}

/* TODO: std::none_of(...) with [](int x) { return x < 0; }. */
inline bool none_negative(const std::vector<int> & /*v*/)
{
    return false;
}

}
