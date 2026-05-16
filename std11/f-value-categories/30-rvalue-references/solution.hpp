#pragma once

/*
 * Step 30 -- rvalue references. See task.md for the spec.
 *
 * Implement two overloads:
 *   int f(int &)   returning 1  -- the "lvalue overload"
 *   int f(int &&)  returning 2  -- the "rvalue overload"
 *
 * Do not change the signatures.
 */

namespace task30
{

inline int f(int & /* x */)
{
    /* TODO: return 1 */
    return 0;
}

inline int f(int && /* x */)
{
    /* TODO: return 2 */
    return 0;
}

}
