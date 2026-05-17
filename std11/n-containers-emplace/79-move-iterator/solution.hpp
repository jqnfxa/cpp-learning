#pragma once

#include <iterator>
#include <string>
#include <vector>

/*
 * Step 79 -- std::move_iterator. See task.md for the spec.
 *
 * Implement two factories that build a destination vector from a
 * source vector, one by copying elements and one by moving them via
 * std::make_move_iterator.
 */

namespace task79
{

/* TODO: return std::vector<std::string>(src.begin(), src.end()). */
inline std::vector<std::string> via_copy_range(std::vector<std::string> & /*src*/)
{
    return {};
}

/* TODO: return std::vector<std::string>(
 *           std::make_move_iterator(src.begin()),
 *           std::make_move_iterator(src.end()));
 */
inline std::vector<std::string> via_move_range(std::vector<std::string> & /*src*/)
{
    return {};
}

}
