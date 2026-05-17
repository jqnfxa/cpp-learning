#pragma once

#include <utility>

/*
 * Step 62 -- decltype. See task.md for the spec.
 *
 * Declare three type aliases using decltype + std::declval.
 */

namespace task62
{

/* TODO: using declared_t = decltype(std::declval<T>()); */
template <class T>
using declared_t = void;

/* TODO: using element_t = decltype(*std::declval<T>().begin()); */
template <class T>
using element_t = void;

/* TODO: using sum_t = decltype(std::declval<A>() + std::declval<B>()); */
template <class A, class B>
using sum_t = void;

}
