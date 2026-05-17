#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

/*
 * Step 83 -- std::move (algorithm) and std::move_backward. See task.md.
 *
 * Implement two functions: a fresh-vector bulk-move via std::move(...,
 * back_inserter(dst)), and an in-place right-shift via
 * std::move_backward over an overlapping range.
 */

namespace task83
{

/* TODO: reserve, then std::move(src.begin(), src.end(),
 *       std::back_inserter(dst)). */
inline std::vector<std::string> move_into_new(std::vector<std::string> & /*src*/)
{
    return {};
}

/* TODO: if v.size() < 2 return; else
 *       std::move_backward(v.begin(), v.end() - 1, v.end()). */
inline void shift_right_by_one(std::vector<std::string> & /*v*/)
{
}

}
