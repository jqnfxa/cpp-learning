#pragma once

#include <type_traits>

/*
 * Step 17 -- noexcept is part of the type (C++17). See task.md.
 *
 * In C++17, `int(*)(int) noexcept` and `int(*)(int)` are
 * DIFFERENT types. Implicit conversion is allowed only in
 * the safe direction (noexcept -> non-noexcept; you can
 * weaken the guarantee but not strengthen it).
 *
 * Three helpers: a noexcept function, a non-noexcept
 * function, and a function that calls a noexcept function
 * through a non-noexcept function pointer.
 *
 * The scaffold returns wrong values; tests catch.
 */

namespace task17
{

inline int squared_noexcept(int x) noexcept
{
    return x * x;
}

inline int squared_throws(int x)
{
    return x * x;
}

/*
 * Call squared_noexcept via a NON-noexcept function pointer.
 * The conversion (noexcept fp -> non-noexcept fp) is legal.
 *
 * Reference body:
 *   int (*f)(int) = &squared_noexcept;   // OK in C++17
 *   return f(v);
 *
 * Stub: returns 0.
 */
inline int call_through_throw_fp(int v)
{
    /* TODO:
     *   int (*f)(int) = &squared_noexcept;
     *   return f(v);
     */
    (void)v;
    return 0;
}

/*
 * Call squared_noexcept via a NOEXCEPT function pointer
 * (matches its type exactly).
 */
inline int call_through_noexcept_fp(int v)
{
    /* TODO:
     *   int (*f)(int) noexcept = &squared_noexcept;
     *   return f(v);
     */
    (void)v;
    return 0;
}

}
