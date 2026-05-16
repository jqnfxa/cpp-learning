#pragma once

#include <algorithm>
#include <initializer_list>
#include <vector>

/*
 * Step 20 -- std::initializer_list. See task.md for the spec.
 *
 * Implement three functions that take a list as a value parameter.
 * Do not change the signatures.
 */

namespace task20
{

inline int sum(std::initializer_list<int> /* values */)
{
    /* TODO: range-for over values and accumulate. */
    return 0;
}

inline int max_of(std::initializer_list<int> /* values */)
{
    /* TODO: return the largest element. Undefined if the list is
     * empty -- the tests never call this with an empty list. */
    return 0;
}

inline std::vector<int> to_vector(std::initializer_list<int> /* values */)
{
    /* TODO: build a vector from the list's iterator pair (or directly
     * from the list -- vector has a constructor that takes one). */
    return std::vector<int>{};
}

}
