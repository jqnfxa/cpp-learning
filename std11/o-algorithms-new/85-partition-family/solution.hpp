#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

/*
 * Step 85 -- is_partitioned / partition_copy / partition_point.
 *
 * Three functions over std::vector<int> using the "x is negative"
 * predicate.
 */

namespace task85
{

/* TODO: std::is_partitioned(v.begin(), v.end(),
 *           [](int x){ return x < 0; }); */
inline bool is_partitioned_by_sign(const std::vector<int> & /*v*/)
{
    return false;
}

/* TODO: std::partition_copy(v.begin(), v.end(),
 *           std::back_inserter(neg), std::back_inserter(nonneg),
 *           [](int x){ return x < 0; }); */
inline std::pair<std::vector<int>, std::vector<int>>
split_by_sign(const std::vector<int> & /*v*/)
{
    return {};
}

/* TODO: std::partition_point(v.begin(), v.end(),
 *           [](int x){ return x < 0; }); convert to index. */
inline std::size_t partition_point_by_sign(const std::vector<int> & /*v*/)
{
    return 0;
}

}
