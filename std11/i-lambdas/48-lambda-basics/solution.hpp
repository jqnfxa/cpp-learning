#pragma once

/*
 * Step 48 -- lambdas: basic syntax and call. See task.md.
 *
 * Implement three functions, each using a lambda expression in
 * its body:
 *   inc(x)            -- captureless lambda taking int
 *   sum(a, b)         -- captureless lambda taking two ints
 *   compute_constant  -- IIFE returning 42
 */

namespace task48
{

inline int inc(int /* x */)
{
    /* TODO:
     *   auto add_one = [](int y) { return y + 1; };
     *   return add_one(x);
     */
    return 0;
}

inline int sum(int /* a */, int /* b */)
{
    /* TODO:
     *   auto add = [](int p, int q) { return p + q; };
     *   return add(a, b);
     */
    return 0;
}

inline int compute_constant()
{
    /* TODO: IIFE. return [](){ return 42; }(); */
    return 0;
}

}
