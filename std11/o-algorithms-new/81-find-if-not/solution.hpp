#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>

/*
 * Step 81 -- std::find_if_not. See task.md for the spec.
 *
 * Implement first_non_whitespace and trim_front using
 * std::find_if_not + an isspace lambda (with unsigned char cast).
 */

namespace task81
{

/* TODO: std::find_if_not(s.begin(), s.end(),
 *           [](unsigned char c){ return std::isspace(c); });
 * then convert the iterator to an index. */
inline std::size_t first_non_whitespace(const std::string & /*s*/)
{
    return 0;
}

/* TODO: same find_if_not call, then return std::string(it, s.end()). */
inline std::string trim_front(const std::string & /*s*/)
{
    return {};
}

}
