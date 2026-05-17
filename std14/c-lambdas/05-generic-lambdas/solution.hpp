#pragma once

#include <string>

/*
 * Step 5 -- generic lambdas (C++14). See task.md.
 *
 * `auto` as a lambda parameter type makes the closure's
 * operator() a TEMPLATE. The lambda can then be called with
 * any argument type and the compiler synthesises a fresh
 * specialisation per call site.
 *
 * Two lambdas at namespace scope:
 *   - square: unary, returns x * x
 *   - concat: binary, returns a + b (works for both arithmetic
 *             and strings)
 *
 * The scaffold ships with both lambda bodies WRONG -- they
 * return one argument unchanged. Tests catch this for every
 * type they're called with.
 *
 * Note: `static auto NAME = [](...){...};` at namespace scope
 * is the C++14 idiom for header-only lambdas. C++17 added
 * inline variables, which would let us write `inline auto`
 * instead.
 */

namespace task05
{

/* TODO: return x * x. */
static auto square = [](auto const &x)
{
    return x;
};

/* TODO: return a + b. */
static auto concat = [](auto const &a, auto const & /*b*/)
{
    return a;
};

}
