#pragma once

#include <cstddef>
#include <string>
#include <vector>

/*
 * Step 17 -- range-for variants. See task.md for the spec.
 *
 * Three functions, each picking the right loop variable form:
 *   double_in_place(v)            -> use `auto &x`
 *   try_double_via_copy(v)        -> use `auto x` (deliberately!)
 *   count_long_strings(v, n)      -> use `auto const &s`
 *
 * Do not change the function signatures.
 */

namespace task17
{

inline void double_in_place(std::vector<int> & /* v */)
{
    /* TODO: range-for with `auto &x : v`, set x *= 2 each iteration. */
}

inline int try_double_via_copy(std::vector<int> & /* v */)
{
    /* TODO: range-for with `auto x : v` (deliberately a copy).
     * Mutate x *= 2 inside the loop and sum the doubled copies.
     * v itself must NOT change. */
    return 0;
}

inline std::size_t count_long_strings(const std::vector<std::string> & /* v */, std::size_t /* min_length */)
{
    /* TODO: range-for with `auto const &s : v`, count s.size() >= min_length. */
    return 0;
}

}
