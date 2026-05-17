#include <gtest/gtest.h>

#include "solution.hpp"

TEST(lambda_basics, inc_basic)
{
    EXPECT_EQ(task48::inc(5), 6);
    EXPECT_EQ(task48::inc(0), 1);
    EXPECT_EQ(task48::inc(-1), 0);
}

TEST(lambda_basics, inc_negative_values)
{
    EXPECT_EQ(task48::inc(-100), -99);
    EXPECT_EQ(task48::inc(99), 100);
}

TEST(lambda_basics, sum_basic)
{
    EXPECT_EQ(task48::sum(3, 4), 7);
    EXPECT_EQ(task48::sum(0, 0), 0);
    EXPECT_EQ(task48::sum(-1, 1), 0);
}

TEST(lambda_basics, sum_negatives)
{
    EXPECT_EQ(task48::sum(-3, -4), -7);
    EXPECT_EQ(task48::sum(-100, 50), -50);
}

TEST(lambda_basics, compute_constant_returns_42)
{
    EXPECT_EQ(task48::compute_constant(), 42);
}

TEST(lambda_basics, compute_constant_is_stable)
{
    /* Called twice, should always return the same value. */
    EXPECT_EQ(task48::compute_constant(), task48::compute_constant());
}
