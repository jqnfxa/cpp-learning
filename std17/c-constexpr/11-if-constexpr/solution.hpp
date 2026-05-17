#pragma once

#include <string>
#include <type_traits>

/*
 * Step 11 -- if constexpr (C++17). See task.md.
 *
 * `if constexpr (cond)` selects ONE branch at compile time
 * based on `cond`. The unselected branch is discarded -- its
 * code doesn't need to compile for the specific instantiation.
 *
 * This is the C++17 replacement for the SFINAE + tag-dispatch
 * patterns that C++11/14 used for compile-time branching.
 *
 * The scaffold ships with stubs that return empty strings.
 */

namespace task11
{

/*
 * describe(v) -- compile-time dispatch on T's type category.
 * Returns a tagged string: "int:N", "float:N", or "other".
 *
 * Reference: if constexpr branches on std::is_integral_v,
 * std::is_floating_point_v.
 *
 * Stub: returns "" -- doesn't branch.
 */
template <class T>
std::string describe(T const &v)
{
    /* TODO:
     *   if constexpr (std::is_integral_v<T>)
     *       return "int:" + std::to_string(v);
     *   else if constexpr (std::is_floating_point_v<T>)
     *       return "float:" + std::to_string(v);
     *   else
     *       return "other";
     */
    (void)v;
    return "";
}

/*
 * size_of(v) -- if T is an std::string, return its size().
 * If T is arithmetic, return 1. Otherwise 0.
 */
template <class T>
std::size_t size_of(T const &v)
{
    /* TODO:
     *   if constexpr (std::is_same_v<T, std::string>)
     *       return v.size();
     *   else if constexpr (std::is_arithmetic_v<T>)
     *       return 1;
     *   else
     *       return 0;
     */
    (void)v;
    return 0;
}

}
