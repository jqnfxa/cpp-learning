#pragma once

#include <algorithm>
#include <initializer_list>
#include <utility>
#include <vector>

/*
 * Step 88 -- std::minmax / std::minmax_element. See task.md.
 *
 * Three thin wrappers around the three minmax forms.
 */

namespace task88
{

/* TODO: return std::minmax(a, b); */
inline std::pair<int, int> minmax_pair(int /*a*/, int /*b*/)
{
    return {0, 0};
}

/* TODO: return std::minmax(xs); */
inline std::pair<int, int> minmax_list(std::initializer_list<int> /*xs*/)
{
    return {0, 0};
}

/* TODO: auto pr = std::minmax_element(v.begin(), v.end());
 *       return {*pr.first, *pr.second}; */
inline std::pair<int, int> minmax_range(const std::vector<int> & /*v*/)
{
    return {0, 0};
}

}
