#pragma once

#include <unordered_set>
#include <vector>

/*
 * Step 76 -- std::unordered_set<T>. See task.md for the spec.
 *
 * Implement dedup (first-seen order preserved) and set_of (range
 * constructor).
 */

namespace task76
{

/* TODO: walk `in`, push to `out` only when seen.insert(x).second is true. */
inline std::vector<int> dedup(const std::vector<int> & /*in*/)
{
    return {};
}

/* TODO: return std::unordered_set<int>(in.begin(), in.end()). */
inline std::unordered_set<int> set_of(const std::vector<int> & /*in*/)
{
    return {};
}

}
