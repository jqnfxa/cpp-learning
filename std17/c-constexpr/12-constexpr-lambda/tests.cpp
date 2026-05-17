#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task12;

TEST(constexpr_lambda, sum_basic)
{
    EXPECT_EQ(t::sum_via_lambda(2, 3), 5);
}

TEST(constexpr_lambda, sum_negative)
{
    EXPECT_EQ(t::sum_via_lambda(-5, 3), -2);
}

TEST(constexpr_lambda, sum_compile_time)
{
    constexpr int s = t::sum_via_lambda(10, 20);
    EXPECT_EQ(s, 30);
}

TEST(constexpr_lambda, factorial_zero)
{
    EXPECT_EQ(t::factorial_via_lambda(0), 1);
}

TEST(constexpr_lambda, factorial_one)
{
    EXPECT_EQ(t::factorial_via_lambda(1), 1);
}

TEST(constexpr_lambda, factorial_five)
{
    EXPECT_EQ(t::factorial_via_lambda(5), 120);
}

TEST(constexpr_lambda, factorial_ten)
{
    EXPECT_EQ(t::factorial_via_lambda(10), 3628800);
}

TEST(constexpr_lambda, factorial_compile_time)
{
    constexpr long long f6 = t::factorial_via_lambda(6);
    EXPECT_EQ(f6, 720);
}

TEST(constexpr_lambda, direct_constexpr_lambda)
{
    /* The textbook usage: a constexpr lambda in a constant
     * expression. */
    constexpr auto sq = [](int x) { return x * x; };
    constexpr int r = sq(7);
    EXPECT_EQ(r, 49);
}
