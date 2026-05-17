#include <gtest/gtest.h>

#include "solution.hpp"

TEST(pack_expansion, min_of_single_arg)
{
    EXPECT_EQ(task56::min_of(3), 3);
    EXPECT_EQ(task56::min_of(-7), -7);
    EXPECT_EQ(task56::min_of(0), 0);
}

TEST(pack_expansion, min_of_two_args)
{
    EXPECT_EQ(task56::min_of(3, 1), 1);
    EXPECT_EQ(task56::min_of(1, 3), 1);
    EXPECT_EQ(task56::min_of(5, 5), 5);
}

TEST(pack_expansion, min_of_many)
{
    EXPECT_EQ(task56::min_of(5, 3, 1, 4, 2), 1);
    EXPECT_EQ(task56::min_of(10, 20, 30, 40, 50), 10);
    EXPECT_EQ(task56::min_of(50, 40, 30, 20, 10), 10);
}

TEST(pack_expansion, min_of_negatives)
{
    EXPECT_EQ(task56::min_of(-3, -1, -5, -2), -5);
    EXPECT_EQ(task56::min_of(-1, 0, 1), -1);
}

TEST(pack_expansion, max_of_single_arg)
{
    EXPECT_EQ(task56::max_of(3), 3);
    EXPECT_EQ(task56::max_of(-7), -7);
}

TEST(pack_expansion, max_of_two_args)
{
    EXPECT_EQ(task56::max_of(3, 1), 3);
    EXPECT_EQ(task56::max_of(1, 3), 3);
}

TEST(pack_expansion, max_of_many)
{
    EXPECT_EQ(task56::max_of(5, 3, 1, 4, 2), 5);
    EXPECT_EQ(task56::max_of(-3, -1, -5, -2), -1);
}

TEST(pack_expansion, mixed_with_doubles)
{
    EXPECT_DOUBLE_EQ(task56::min_of(3.14, 2.71, 1.41), 1.41);
    EXPECT_DOUBLE_EQ(task56::max_of(3.14, 2.71, 1.41), 3.14);
}
