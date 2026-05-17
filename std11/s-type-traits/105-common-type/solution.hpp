#pragma once

#include <type_traits>

/*
 * Step 105 -- std::common_type. See task.md for the spec.
 *
 * Variadic max via three overloads (1-arg base, 2-arg base,
 * N-arg recursion).
 */

namespace task105
{

/* TODO: return x; */
template <class T>
T max(T x)
{
    (void)x;
    return T{};
}

/* TODO:
 *   using R = typename std::common_type<A, B>::type;
 *   return a < b ? static_cast<R>(b) : static_cast<R>(a);
 */
template <class A, class B>
typename std::common_type<A, B>::type max(A /*a*/, B /*b*/)
{
    return typename std::common_type<A, B>::type{};
}

/* TODO: return max(max(a, b), rest...); */
template <class A, class B, class... Rest>
typename std::common_type<A, B, Rest...>::type max(A /*a*/, B /*b*/, Rest... /*rest*/)
{
    return typename std::common_type<A, B, Rest...>::type{};
}

}
