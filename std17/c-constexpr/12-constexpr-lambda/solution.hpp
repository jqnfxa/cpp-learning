#pragma once

/*
 * Step 12 -- constexpr lambdas (C++17). See task.md.
 *
 * C++17 made lambda operator() IMPLICITLY constexpr when
 * its body is a constant expression. You can use the lambda
 * in constant-expression contexts like static_assert and as
 * the initialiser of a constexpr variable.
 *
 * Two demos. Scaffold uses non-constexpr-friendly bodies
 * that return zero -- tests catch.
 */

namespace task12
{

/*
 * Sum-by-lambda: define a lambda that adds two ints, and
 * call it. Works at compile time when args are constants.
 */
inline constexpr int sum_via_lambda(int a, int b)
{
    /* TODO:
     *   constexpr auto add = [](int x, int y) { return x + y; };
     *   return add(a, b);
     */
    (void)a;
    (void)b;
    return 0;
}

/*
 * factorial_via_lambda(n) -- compute n! by repeatedly
 * applying a lambda inside a constexpr function. Uses
 * relaxed-constexpr loop (step 1 of std14).
 */
inline constexpr long long factorial_via_lambda(int n)
{
    /* TODO:
     *   constexpr auto step = [](long long acc, int i) { return acc * i; };
     *   long long acc = 1;
     *   for (int i = 2; i <= n; ++i) acc = step(acc, i);
     *   return acc;
     */
    (void)n;
    return 0;
}

}
