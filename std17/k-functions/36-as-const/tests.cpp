#include <gtest/gtest.h>

#include "solution.hpp"

TEST(AsConst, returns_size)
{
    EXPECT_EQ(task36::count_with_as_const(std::vector<int>{1, 2, 3}), 3u);
}

TEST(AsConst, empty)
{
    EXPECT_EQ(task36::count_with_as_const({}), 0u);
}
