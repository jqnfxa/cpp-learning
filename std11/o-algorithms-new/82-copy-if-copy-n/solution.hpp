#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>

/*
 * Step 82 -- std::copy_if and std::copy_n. See task.md for the spec.
 *
 * Implement `evens` via copy_if with a `% 2 == 0` lambda, and `first_n`
 * via copy_n with the source length clamped.
 */

namespace task82
{

/* TODO: std::copy_if(v.begin(), v.end(), std::back_inserter(out),
 *                    [](int x) { return x % 2 == 0; }); */
inline std::vector<int> evens(const std::vector<int> & /*v*/)
{
    return {};
}

/* TODO: clamp k to v.size(), then std::copy_n(v.begin(), n,
 *       std::back_inserter(out)); */
inline std::vector<int> first_n(const std::vector<int> & /*v*/, std::size_t /*k*/)
{
    return {};
}

}
