#pragma once

#include <cstddef>

/*
 * Step 15 -- range-for over arrays. See task.md for the spec.
 *
 * Fill in the bodies of the two function templates below with a
 * range-for loop. Do not change the signatures.
 */

namespace task15
{

template <std::size_t N>
int sum(const int (&arr)[N])
{
    /* TODO: walk the array with `for (int x : arr)` and accumulate. */
    (void)arr;
    return 0;
}

template <std::size_t N>
std::size_t count_evens(const int (&arr)[N])
{
    /* TODO: walk the array with `for (int x : arr)` and count
     * elements where x % 2 == 0. */
    (void)arr;
    return 0;
}

}
