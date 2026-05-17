#pragma once

#include <tuple>

/*
 * Step 34 -- std::apply (C++17). Unpack a tuple into a
 * function's args. Replaces the C++14 index_sequence helper.
 */

namespace task34
{

inline int sum3(int a, int b, int c)
{
    return a + b + c;
}

/* TODO: return std::apply(sum3, t); */
inline int sum_via_apply(std::tuple<int, int, int> t)
{
    (void)t;
    return 0;
}

/* TODO: return std::apply([](int a, int b){ return a*b; }, t); */
inline int multiply_via_apply(std::tuple<int, int> t)
{
    (void)t;
    return 0;
}

}
