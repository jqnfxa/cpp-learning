#pragma once

#include <vector>

/*
 * Step 61 -- trailing return type. See task.md.
 *
 * Three function templates declared `auto NAME(...) -> decltype(...)`.
 */

namespace task61
{

/* TODO: auto add(A a, B b) -> decltype(a + b) { return a + b; } */
template <class A, class B>
A add(A a, B /* b */)
{
    return a;
}

/* TODO: auto multiply(A a, B b) -> decltype(a * b) { return a * b; } */
template <class A, class B>
A multiply(A a, B /* b */)
{
    return a;
}

/* TODO: auto first_element(const std::vector<T> &v) -> decltype(v[0]) {
 *           return v[0];
 *       }
 */
template <class T>
T first_element(const std::vector<T> &v)
{
    return v[0];
}

}
