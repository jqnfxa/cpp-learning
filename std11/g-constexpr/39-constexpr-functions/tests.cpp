#include <gtest/gtest.h>

#include "solution.hpp"

/*
 * Compile-time block. If any function isn't actually constexpr
 * (or returns the wrong value), these fire before any TEST runs.
 */

/* factorial */
static_assert(task39::factorial(0) == 1, "0! == 1");
static_assert(task39::factorial(1) == 1, "1! == 1");
static_assert(task39::factorial(2) == 2, "2! == 2");
static_assert(task39::factorial(5) == 120, "5! == 120");
static_assert(task39::factorial(10) == 3628800, "10! == 3628800");

/* fib */
static_assert(task39::fib(0) == 0, "fib(0) == 0");
static_assert(task39::fib(1) == 1, "fib(1) == 1");
static_assert(task39::fib(2) == 1, "fib(2) == 1");
static_assert(task39::fib(7) == 13, "fib(7) == 13");
static_assert(task39::fib(10) == 55, "fib(10) == 55");

/* gcd */
static_assert(task39::gcd(12, 18) == 6, "gcd(12, 18) == 6");
static_assert(task39::gcd(7, 0) == 7, "gcd(7, 0) == 7");
static_assert(task39::gcd(0, 7) == 7, "gcd(0, 7) == 7");
static_assert(task39::gcd(-12, 18) == 6, "gcd(-12, 18) == 6");
static_assert(task39::gcd(1, 1) == 1, "gcd(1, 1) == 1");
static_assert(task39::gcd(0, 0) == 0, "gcd(0, 0) == 0");

TEST(constexpr_fns, factorial_runtime)
{
    for (int n = 0; n <= 6; ++n)
    {
        int expected = 1;
        for (int i = 2; i <= n; ++i)
        {
            expected *= i;
        }
        EXPECT_EQ(task39::factorial(n), expected) << "n = " << n;
    }
}

TEST(constexpr_fns, fib_runtime)
{
    const int expected[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
    for (int i = 0; i < 11; ++i)
    {
        EXPECT_EQ(task39::fib(i), expected[i]) << "fib(" << i << ")";
    }
}

TEST(constexpr_fns, gcd_runtime)
{
    EXPECT_EQ(task39::gcd(48, 18), 6);
    EXPECT_EQ(task39::gcd(100, 75), 25);
    EXPECT_EQ(task39::gcd(17, 13), 1);
    EXPECT_EQ(task39::gcd(-30, 12), 6);
    EXPECT_EQ(task39::gcd(30, -12), 6);
}
