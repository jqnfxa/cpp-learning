#pragma once

#include <type_traits>

/*
 * Step 37 -- reference collapsing. See task.md for the spec.
 *
 * Implement two helpers:
 *   kind<T>()        -- string tag for what T is
 *   deduce(T &&arg)  -- forwarding-reference; returns kind<T>()
 *                       where T was deduced from arg.
 */

namespace task37
{

template <class T>
const char *kind()
{
    /* TODO: return "lvalue_ref" / "rvalue_ref" / "value" based on T. */
    return "?";
}

template <class T>
const char *deduce(T && /* arg */)
{
    /* TODO: return kind<T>(). */
    return "?";
}

}
