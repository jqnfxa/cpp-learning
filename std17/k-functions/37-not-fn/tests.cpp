#include <gtest/gtest.h>

#include "solution.hpp"

TEST(NotFn, mixed)
{
    EXPECT_EQ(task37::count_non_positive({1, -2, 3, 0, -5}), 3u);
}

TEST(NotFn, all_positive)
{
    EXPECT_EQ(task37::count_non_positive({1, 2, 3}), 0u);
}

TEST(NotFn, all_negative)
{
    EXPECT_EQ(task37::count_non_positive({-1, -2, -3}), 3u);
}
