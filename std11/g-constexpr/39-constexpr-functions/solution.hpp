#pragma once

/*
 * Step 39 -- constexpr functions (C++11 strict). See task.md.
 *
 * Implement three single-return constexpr functions:
 *   factorial(n)   -- n! for n >= 0
 *   fib(n)         -- nth Fibonacci (fib(0)=0, fib(1)=1)
 *   gcd(a, b)      -- greatest common divisor of |a|, |b|
 *
 * Each body is one `return` expression. Use ternary + recursion.
 */

namespace task39
{

constexpr int factorial(int /* n */)
{
    /* TODO: return n <= 1 ? 1 : n * factorial(n - 1); */
    return 0;
}

constexpr int fib(int /* n */)
{
    /* TODO: return n < 2 ? n : fib(n - 1) + fib(n - 2); */
    return 0;
}

constexpr int gcd(int /* a */, int /* b */)
{
    /* TODO: return b == 0 ? (a < 0 ? -a : a) : gcd(b, a % b); */
    return 0;
}

}
