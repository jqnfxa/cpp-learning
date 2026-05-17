#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace task37
{

inline bool is_positive(int x)
{
    return x > 0;
}

/* TODO: return std::count_if(v.begin(), v.end(), std::not_fn(is_positive)); */
/* TODO: return std::count_if(v.begin(), v.end(), std::not_fn(is_positive)); */
inline std::size_t count_non_positive(const std::vector<int> &v)
{
    (void)v;
    return 0;
}

}
