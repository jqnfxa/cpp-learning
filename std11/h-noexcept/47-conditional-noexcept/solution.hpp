#pragma once

#include <utility>

/*
 * Step 47 -- conditional noexcept. See task.md for the spec.
 *
 * Implement swap_ as a function template whose noexcept-ness
 * depends on T's move ctor and move-assign being noexcept.
 */

namespace task47
{

/* TODO: add the conditional noexcept clause to this signature:
 *   noexcept(noexcept(T(std::move(a))) && noexcept(a = std::move(b)))
 */
template <class T>
void swap_(T &a, T &b)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

}
