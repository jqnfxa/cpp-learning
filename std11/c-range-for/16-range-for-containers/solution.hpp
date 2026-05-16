#pragma once

#include <string>
#include <vector>

/*
 * Step 16 -- range-for over containers. See task.md for the spec.
 *
 * Implement both functions with a range-for loop. Do not change the
 * signatures.
 */

namespace task16
{

inline int sum(const std::vector<int> & /* v */)
{
    /* TODO: walk v with range-for and accumulate. */
    return 0;
}

inline std::string join(const std::vector<std::string> & /* parts */, const std::string & /* sep */)
{
    /* TODO: walk parts with range-for and concatenate with sep
     * placed *between* elements (not before, not after). */
    return "";
}

}
