#pragma once

#include <utility>
#include <vector>

/*
 * Step 18 -- list initialization. See task.md for the spec.
 *
 * Each function returns a value built with the C++11 brace-init
 * form, T{...}. Do not change the signatures.
 */

namespace task18
{

inline int make_int(int /* n */)
{
    /* TODO: return int{n}; */
    return 0;
}

inline int make_default_int()
{
    /* TODO: return int{}; (value-initialized -- this is 0) */
    return -1;
}

inline std::pair<int, int> make_pair_of(int /* a */, int /* b */)
{
    /* TODO: return std::pair<int, int>{a, b}; */
    return std::pair<int, int>{};
}

inline std::vector<int> make_vec3(int /* a */, int /* b */, int /* c */)
{
    /* TODO: return std::vector<int>{a, b, c}; */
    return std::vector<int>{};
}

}
