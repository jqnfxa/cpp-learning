#pragma once

#include <vector>

/*
 * Step 3 -- return-type deduction `auto f(...)`. See task.md.
 *
 * C++14 lets you write `auto add(A a, B b) { return a + b; }`
 * and the compiler deduces the return type from the return
 * expression. C++11 required the trailing-return-type form
 * `auto add(A a, B b) -> decltype(a + b)` to express the same
 * idea (cf. std11 step 61).
 *
 * The scaffold ships with bodies that return `a` only -- so the
 * VALUE is wrong and the deduced TYPE is wrong (A instead of
 * decltype(a+b)). Both runtime-value and is_same type tests
 * detect that.
 */

namespace task03
{

/* TODO: return a + b. With `auto`, the return type is deduced
 * from the return expression -- it becomes decltype(a + b),
 * exactly what the C++11 trailing form spelled out. */
template <class A, class B>
auto add(A a, B /*b*/)
{
    return a;
}

/* TODO: return a * b. */
template <class A, class B>
auto multiply(A a, B /*b*/)
{
    return a;
}

/* TODO: return a - b. */
template <class A, class B>
auto subtract(A a, B /*b*/)
{
    return a;
}

}
