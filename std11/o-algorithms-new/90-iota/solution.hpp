#pragma once

#include <cstddef>
#include <numeric>
#include <vector>

/*
 * Step 90 -- std::iota. See task.md for the spec.
 *
 * Implement two factories that produce monotonically-increasing
 * vectors.
 */

namespace task90
{

/* TODO: std::vector<int> v(n); std::iota(v.begin(), v.end(), 0); return v; */
inline std::vector<int> range_n(std::size_t /*n*/)
{
    return {};
}

/* TODO: same shape with start as the seed. */
inline std::vector<int> range_from(std::size_t /*n*/, int /*start*/)
{
    return {};
}

}
