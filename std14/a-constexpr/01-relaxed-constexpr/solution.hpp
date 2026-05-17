#pragma once

/*
 * Step 1 -- relaxed C++14 constexpr. See task.md for the spec.
 *
 * Rewrite the C++11 single-return-only factorial using a regular
 * iterative loop INSIDE a constexpr function. C++14 allows
 * multiple statements, local variables, `if`, and loops in a
 * constexpr body.
 *
 * The scaffold ships with the OLD (C++11) recursive form so the
 * function compiles, but the tests check a behavior the stub
 * gets wrong on purpose: factorial(0) and factorial(1) both
 * have to equal 1, and the stub omits that base case.
 */

namespace task01
{

/* TODO: rewrite as:
 *   constexpr long long factorial(long long n) {
 *       long long r = 1;
 *       for (long long i = 2; i <= n; ++i) r *= i;
 *       return r;
 *   }
 *
 * Note: the C++11 form below is allowed in C++14 too, but the
 * point of this step is to use the new RELAXED grammar -- local
 * variables and a loop -- which C++11 forbade in a constexpr body.
 */
constexpr long long factorial(long long n)
{
    /* Stub: wrong base case. Returns 0 for n<=0 so the product
     * chain collapses to 0 for every n. Terminates (so compile-
     * time evaluation still works), but answers are all 0. */
    return n <= 0 ? 0 : n * factorial(n - 1);
}

}
