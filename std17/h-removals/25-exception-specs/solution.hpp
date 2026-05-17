#pragma once

#include <stdexcept>

/*
 * Step 25 -- removed: dynamic exception specs (throw(X, Y))
 * and std::unexpected. Use `noexcept` / `noexcept(false)`.
 */

namespace task25
{

/* Replaces `int may_throw() throw(std::runtime_error)`.
 *   C++17 form: `int may_throw() noexcept(false)` (or just
 *   leave the spec off -- the default is "may throw"). */
inline int may_throw(bool fail) noexcept(false)
{
    if (fail)
    {
        throw std::runtime_error("fail");
    }
    return 42;
}

/* Replaces `int wont_throw() throw()`.
 *   C++17 form: `int wont_throw() noexcept`. */
/* TODO: add `noexcept` to the function. */
inline int wont_throw() noexcept
{
    return 99;
}

}
