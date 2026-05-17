#pragma once

#include <utility>
#include <vector>

namespace task36
{

inline std::size_t count_via_const(const std::vector<int> &v)
{
    return v.size();
}

/* TODO: return count_via_const(std::as_const(v)); */
/* TODO: return count_via_const(std::as_const(v)); */
inline std::size_t count_with_as_const(std::vector<int> v)
{
    (void)v;
    return 0;
}

}
