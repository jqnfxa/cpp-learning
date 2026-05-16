#pragma once

#include <stdexcept>
#include <string>

/*
 * Step 10 -- [[noreturn]] attribute. See task.md for the spec.
 *
 * Two things to fix in panic() below:
 *   1. Add the [[noreturn]] attribute to its declaration.
 *   2. Throw a std::runtime_error(msg) in the body.
 *
 * positive_only() is already implemented and depends on panic being
 * [[noreturn]]: with the attribute, the compiler accepts that
 * control never returns from the panic() call. Without it, the
 * task's CMakeLists turns the "control reaches end of non-void"
 * warning into an error.
 */

namespace task10
{

/* TODO 1: add [[noreturn]] here.
 * TODO 2: throw std::runtime_error(msg) instead of returning. */
inline void panic(const std::string &msg)
{
    (void)msg;
}

inline int positive_only(int n)
{
    if (n >= 0)
    {
        return n;
    }
    panic("expected non-negative input");
}

}
