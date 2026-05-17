#pragma once

#include <algorithm>
#include <string>

/*
 * Step 89 -- std::is_permutation. See task.md for the spec.
 *
 * Implement an anagram check using the C++11 3-iterator overload,
 * preceded by the required length-equality guard.
 */

namespace task89
{

/* TODO:
 *   if (a.size() != b.size()) return false;
 *   return std::is_permutation(a.begin(), a.end(), b.begin());
 */
inline bool is_anagram(const std::string & /*a*/, const std::string & /*b*/)
{
    return false;
}

}
