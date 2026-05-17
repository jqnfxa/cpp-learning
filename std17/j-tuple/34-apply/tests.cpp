#include <gtest/gtest.h>

#include "solution.hpp"

TEST(Apply, sum3)
{
    EXPECT_EQ(task34::sum_via_apply({1, 2, 3}), 6);
}

TEST(Apply, sum3_negatives)
{
    EXPECT_EQ(task34::sum_via_apply({10, -5, -2}), 3);
}

TEST(Apply, multiply2)
{
    EXPECT_EQ(task34::multiply_via_apply({3, 4}), 12);
}

TEST(Apply, multiply_with_zero)
{
    EXPECT_EQ(task34::multiply_via_apply({0, 5}), 0);
}
