#pragma once

#include <cstddef>
#include <iterator>
#include <vector>

/*
 * Step 93 -- std::next and std::prev. See task.md for the spec.
 *
 * Implement two element-access functions using next/prev rather
 * than direct subscript or manual iterator arithmetic.
 */

namespace task93
{

/* TODO: *std::next(v.begin(), static_cast<std::ptrdiff_t>(k)); */
inline int nth_element(const std::vector<int> & /*v*/, std::size_t /*k*/)
{
    return 0;
}

/* TODO: *std::prev(v.end(), static_cast<std::ptrdiff_t>(k)); */
inline int nth_from_end(const std::vector<int> & /*v*/, std::size_t /*k*/)
{
    return 0;
}

}
