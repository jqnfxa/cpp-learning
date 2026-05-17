#pragma once

#include <vector>

/*
 * Step 4 -- decltype(auto). See task.md.
 *
 * Two parallel functions return v[0]. With bare `auto`,
 * references get stripped and the return type is T (a copy).
 * With `decltype(auto)`, the return type follows decltype rules
 * and preserves the T& that v[0] yields.
 *
 * The scaffold ships with BOTH functions using `auto` -- so
 * decltype_first returns by VALUE when it should return by
 * reference. Tests detect that via address comparison and
 * is_same<...>::value checks on the deduced type.
 */

namespace task04
{

template <class T>
auto auto_first(std::vector<T> &v)
{
    /* Bare `auto`: deduction strips the reference. v[0] is
     * T&, so this returns T by value (a COPY). */
    return v[0];
}

/* TODO: change the return type from `auto` to `decltype(auto)`.
 * Same body, but the return type follows decltype rules and
 * preserves the reference that v[0] yields. */
template <class T>
auto decltype_first(std::vector<T> &v)
{
    return v[0];
}

}
